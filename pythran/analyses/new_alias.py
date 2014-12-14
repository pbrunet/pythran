""" Aliases gather aliasing informations. """

from pythran.analyses.global_declarations import GlobalDeclarations
from pythran.intrinsic import Intrinsic, Class, NewMem
from pythran.passmanager import ModuleAnalysis
from pythran.syntax import PythranSyntaxError
from pythran.tables import functions, methods, MODULES
from pythran import utils
import pythran.metadata as md

import ast
import itertools
from collections import defaultdict


class Alias(object):
    def __init__(self, origin):
        self.origin = origin

    def __eq__(self, other):
        return self.origin == other.origin

    def __hash__(self):
        return hash(str(self))

class NamedAlias(Alias):
    def __str__(self):
        return "Name(" + str(self.origin) + ")"

    __repr__ = __str__

class Content(Alias):
    def __str__(self):
        return "Content(" + str(self.origin) + ")"
    __repr__ = __str__

class HeapAlias(Alias):
    def __init__(self, origin, *combine):
        self.combine = combine
        super(HeapAlias, self).__init__(self, origin)

    def __str__(self):
        return "Content(" + str(self.origin) + ")"
    __repr__ = __str__

class Key(Alias):
    def __str__(self):
        return "Content(" + str(self.origin) + ")"
    __repr__ = __str__


class NewAliases(ModuleAnalysis):
    """Gather aliasing informations across nodes.

    Examples
    --------
    >>> import ast
    >>> from pythran import passmanager, backend
    >>> pm = passmanager.PassManager("test")
    >>> code = '''
    ... def foo(a, b):
    ...     return a
    ... '''
    >>> node = ast.parse(code)
    >>> res = pm.gather(NewAliases, node)
    >>> (res[node.body[0].body[0].value][NamedAlias('a')] ==
    ...  {NamedAlias('a'), NamedAlias('b'), NamedAlias('foo'), node.body[0]})
    True
    >>> code = '''
    ... def foo(a, b):
    ...     a = 1
    ...     return a
    ... '''
    >>> node = ast.parse(code)
    >>> res = pm.gather(NewAliases, node)
    >>> (res[node.body[0].body[1].value][NamedAlias('a')] ==
    ...  {node.body[0].body[0].value, NamedAlias('a')})
    True
    >>> code = '''
    ... def foo(a, b):
    ...     a[b] = 1
    ...     return a
    ... '''
    >>> node = ast.parse(code)
    >>> res = pm.gather(NewAliases, node)
    >>> (res[node.body[0].body[1].value][NamedAlias('a')] ==
    ...  {NamedAlias('a'), NamedAlias('b'), NamedAlias('foo'), node.body[0]})
    True
    >>> (res[node.body[0].body[1].value][Content(NamedAlias('a'))] ==
    ...  {Content(NamedAlias('a')), Content(NamedAlias('b')),
    ...   Content(NamedAlias('foo')), node.body[0].body[0].value})
    True
    """

    NO_CONTENT = (ast.Num, ast.FunctionDef)

    @staticmethod
    def copy(aliases):
        return {k: {alias for alias in v} for k, v in aliases.iteritems()}

    def visit(self, node):
        self.result[node] = self.copy(self.aliases)
        return super(NewAliases, self).visit(node)

    def __init__(self):
        self.result = dict()
        self.aliases = defaultdict(set)
        super(NewAliases, self).__init__(GlobalDeclarations)

    def expand_unknown(self, node):
        # should include built-ins too?
        unkowns = {NewMem()}.union(self.global_declarations.values())
        return unkowns.union(node.args)

    def visit_FunctionDef(self, node):
        """
        Initialise aliasing default value before visiting.

        Add aliasing values for :
            - Pythonic
            - globals declarations
            - current function arguments
        """
        self.aliases = defaultdict(set)

        def save_intrinsic_alias(name, module):
            """ Recursively save default aliases for pythonic functions. """
            for n, v in module.iteritems():
                new_name = name + (n,)
                if isinstance(v, dict):  # Submodules case
                    save_intrinsic_alias(new_name, v)
                else:
                    self.aliases[".".join(new_name)] = {v}
                    if isinstance(v, Class):
                        save_intrinsic_alias(new_name, v.fields)

        #save_intrinsic_alias((), MODULES)
        self.aliases.update((NamedAlias(f.name), {f})
                            for f in self.global_declarations.itervalues())

        other_args = {NamedAlias(node.name), node}.union(
            {NamedAlias(arg.id) for arg in node.args.args})
        # all args may alias on another arg
        self.aliases.update((NamedAlias(arg.id), other_args)
                            for arg in node.args.args)
        self.aliases[NamedAlias(node.name)].update(other_args)
        map(self.visit, node.body)

    def add(self, to, from_, and_back=True):
        """
        add an alias to another one.
        Alias can be : NamedAlias, Content, HeapAlias, Key
        """
        if to is from_:
            return
        init_from = from_
        if isinstance(to, Alias):
            for alias in [i for i in self.aliases[to]]:
                from_ = init_from
                for i in xrange(10):
                    self.aliases[alias].add(from_)
                    if isinstance(from_, NewAliases.NO_CONTENT) or isinstance(to, NewAliases.NO_CONTENT):
                        break
                    from_ = Content(from_)
                    alias = Content(alias)
        if and_back:
            self.add(init_from, to, False)

    def remove(self, alias):
        for i in xrange(10):
            if alias not in self.aliases:
                alias = Content(alias)
                continue
            del self.aliases[alias]
            for v in self.aliases.itervalues():
                if alias in v:
                    v.discard(alias)
            if not isinstance(alias, Alias):
                return
            alias = Content(alias)

    def visit_Assign(self, node):
        """
        Compute aliasing modification from assignment.

        if we assign a value, new possible alias is:
            - all others assigned values (as we may assign on multiple values.
            - aliasing created by rvalue

        previous aliasing on assigned value is removed.
        """
        value_aliases = self.visit(node.value)
        for t in node.targets:
            # Handle subscript
            v_alias = NamedAlias(utils.get_variable(t).id)
            for i in xrange(utils.get_depth(t)):
                v_alias = Content(v_alias)
            # new aliasing contains new variables as : a = b = 1 means `a` and
            # `b` are aliasing.
            value_aliases.add(v_alias)
            # Previous aliasing is removed
            self.remove(v_alias)
            # A variable always alias on itself
            self.aliases[v_alias].add(v_alias)

        for t in node.targets:
            # Update all others variable.
            # Be careful, with a = b; a[i] = 1, we have to update both
            # Content(a) and Content(b). It is done thanks to visitor and
            # product.
            for a, b in itertools.product(self.visit(t), value_aliases):
                self.add(a, b)

    def visit_AugAssign(self, node):
        for a, b in itertools.product(self.visit(node.target), self.visit(node.value)):
            if isinstance(a, NewAliases.NO_CONTENT) or isinstance(b, NewAliases.NO_CONTENT):
                continue
            self.add(Content(a), Content(b))

    def visit_For(self, node):
        # We may loop until start of loop become fix
        target = NamedAlias(node.target.id)
        self.remove(target)
        self.add(target, Content(node.iter))
        self.generic_visit(node)

    def visit_If(self, node):
        md.visit(self, node)
        self.visit(node.test)
        before_alias = self.copy(self.aliases)

        map(self.visit, node.body)

        self.aliases, before_alias  = before_alias, self.aliases
        map(self.visit, node.orelse)

        for k, v in before_alias.iteritems():
            if k in self.aliases:
                self.aliases[k].update(v)
            else:
                assert isinstance(v, set)
                self.aliases[k] = v

    def visit_BoolOp(self, node):
        return set.union(*map(self.visit, node.values))

    def visit_UnaryOp(self, node):
        self.generic_visit(node)
        return {node}

    visit_Compare = visit_UnaryOp

    def visit_BinOp(self, node):
        """ a = [sum] + [prod] => content(a) may alias on sum or foo """
        return HeapAlias(node, self.visit(node.right), self.visit(node.left))

    def visit_IfExp(self, node):
        self.visit(node.test)
        rec = map(self.visit, (node.body, node.orelse))
        return set.union(*rec)

    def visit_Dict(self, node):
        new_alias = HeapAlias(node)
        self.aliases[new_alias] = {new_alias}
        for elt in node.elts:
            for alias in self.visit(elt):
                self.add(Content(new_alias), alias)
        for key in node.keys:
            for alias in self.visit(key):
                self.add(Key(new_alias), alias)
        return {new_alias}

    def visit_Set(self, node):
        new_alias = HeapAlias(node)
        self.aliases[new_alias] = {new_alias}
        for elt in node.elts:
            for alias in self.visit(elt):
                self.add(Content(new_alias), alias)
        return {new_alias}

    visit_List = visit_Set
    visit_Tuple = visit_Set

    def visit_ListComp(self, node):
        new_alias = HeapAlias(node)
        self.aliases[new_alias] = {new_alias}
        map(self.visit, node.generators)
        for alias in self.visit(node.elt):
            self.add(Content(new_alias), alias)
        return {new_alias}

    visit_SetComp = visit_ListComp

    def visit_DictComp(self, node):
        new_alias = HeapAlias(node)
        self.aliases[new_alias] = {new_alias}
        map(self.visit, node.generators)
        for alias in self.visit(node.value):
            self.add(Content(new_alias), alias)
        for alias in self.visit(node.key):
            self.add(Key(new_alias), alias)
        return {new_alias}

    def visit_GeneratorExp(self, node):
        current_alias, self.aliases = self.aliases, self.copy(self.aliases)
        new_alias = HeapAlias(node)
        self.aliases[new_alias] = {new_alias}
        map(self.visit, node.generators)
        for alias in self.visit(node.elt):
            self.add(Content(new_alias), alias)
        self.aliases = current_alias
        return {new_alias}

    def call_return_alias(self, node):
        func = node.func
        aliases = set()
        if isinstance(func, ast.Attribute):
            # It is an intrinsic function
            _, signature = methods.get(func.attr, functions[func.attr])
            aliases = signature.return_alias(node)
        elif isinstance(func, ast.Name):
            func_aliases = self.aliases[func]
            for func_alias in func_aliases:
                signature = None
                if isinstance(func_alias, Intrinsic):
                    aliases = func_alias.return_alias(node)
                elif isinstance(func_alias, ast.FunctionDef):
                    # We should compute a return alias for user written
                    # functions
                    pass
                else:
                    # We may prone some impossible aliases
                    pass  # better thing to do ?
        return aliases or self.expand_unknown(node)

    def visit_Call(self, node):
        self.generic_visit(node)
        f = node.func
        # special handler for bind functions
        if isinstance(f, ast.Attribute) and f.attr == "partial":
            return {node}
        else:
            return_alias = self.call_return_alias(node)
            # expand collected aliases
            all_aliases = set()
            for value in return_alias:
                if isinstance(value, NewMem):
                    all_aliases.add(value)
                elif value in self.aliases:
                    all_aliases.update(self.aliases[value])
                else:
                    raise NotImplementedError
            return all_aliases

    visit_Num = visit_UnaryOp
    visit_Str = visit_UnaryOp

    def visit_Attribute(self, node):
        return {node}

    def visit_Subscript(self, node):
        self.generic_visit(node)
        return {Content(alias) for alias in self.visit(node.value)
                if not isinstance(alias, NewAliases.NO_CONTENT)}

    def visit_Name(self, node):
        return {i for i in self.aliases[NamedAlias(node.id)]}

    def visit_comprehension(self, node):
        target = NamedAlias(node.target.id)
        self.remove(target)
        self.add(target, Content(node.iter))
        self.generic_visit(node)

    def visit_ExceptHandler(self, node):
        if node.name:
            target = NamedAlias(node.name.id)
            self.remove(target)
            self.add(target, node.name)
        self.generic_visit(node)
