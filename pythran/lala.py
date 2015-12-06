""" TypingTest replaces builtins by their full paths. """

from pythran.analyses import Globals, Locals
from pythran.passmanager import Transformation
from pythran.syntax import PythranSyntaxError
from pythran.tables import MODULES
from pythran import metadata as md

import string
import ast

def naming():
    k = 0
    while True:
        for a in string.ascii_lowercase:
            yield ("'"+a+str(k)) if (k > 0) else (a)
        k = k+1


class TypingTest(Transformation):

    """
    Expands all builtins into full paths.

    >>> import ast
    >>> from pythran import passmanager, backend
    >>> node = ast.parse("def foo(): return list()")
    >>> pm = passmanager.PassManager("test")
    >>> _, node = pm.apply(TypingTest, node)
    >>> print pm.dump(backend.Python, node)
    def foo():
        return __builtin__.list()
    """

    def __init__(self):
        # (a, b, c) => type a is the same as b in depth c : b[][][][] = a
        self.constraints = []
        self.env = {}
        self.fun_env = {}
        self.names = naming()
        Transformation.__init__(self)

    def fresh(self):
        """ declare a new variable. """
        return md.TVar('$' + next(self.names))

    def visit_Return(self, node):
        node.value = self.visit(node.value)
        ret_ty = md.get(node.value, md.TType)[0]
        self.constraints += [(ret_ty, self.retty)]
        self.retty = ret_ty
        return node

    def visit_Num(self, node):
        md.add(node, md.TLong())
        return node

    def visit_Compare(self, node):
        node.left = self.visit(node.left)
        left_ty = md.get(node.left, md.TType)[0]
        node.comparators = map(self.visit, node.comparators)
        for comp in node.comparators:
            comp_ty = md.get(comp, md.TType)[0]
            self.constraints += [(left_ty, comp_ty)]

        md.add(node, md.TBool())
        return node

    def visit_BoolOp(self, node):
        other_ty = None
        values = list()
        for value in node.values:
            values.append(self.visit(value))
            new_ty = md.get(values[-1], md.TType)[0]
            if other_ty:
                self.constraints += [(other_ty, new_ty)]
            other_ty = new_ty
        node.values = values
        md.add(node, other_ty)
        return node

    def visit_UnaryOp(self, node):
        node.operand = self.visit(node.operand)
        op_ty = md.get(node.operand, md.TType)[0]
        md.add(node, op_ty)
        return node

    def visit_Import(self, node):
        for alias in node.names:
            self.env[alias.asname or alias.name] = md.TModule(alias.asname or alias.name)
        return node

    def visit_BinOp(self, node):
        node.left = self.visit(node.left)
        node.right = self.visit(node.right)
        left_ty = md.get(node.left, md.TType)[0]
        right_ty = md.get(node.right, md.TType)[0]
        self.constraints += [(left_ty, right_ty)]
        md.add(node, left_ty)
        return node

    def visit_FunctionDef(self, node):
        for k, v in self.env.items():
            if not isinstance(v, (md.TFun, md.TModule)):
                del self.env[k]
        # Type des arguments
        self.argtys = [self.fresh() for v in node.args.args]
        # Type de retour
        self.retty = md.TVar("$retty" + node.name)
        # Initialise le type des parametres
        self.env[node.name] = md.TFun(self.retty, *self.argtys)
        for (arg, ty) in zip(node.args.args, self.argtys):
            self.env[arg.id] = ty

        map(self.visit, node.body)
        self.fun_env[node.name] = self.env.copy()
        return node

    def get_name(self, node):
        if isinstance(node, ast.Name):
            return node.id
        else:
            assert isinstance(node, ast.Subscript)
            # FIXME handl later            node.slice
            return self.get_name(node.value)

    def visit_Assign(self, node):
        node.value = self.visit(node.value)
        val_ty = md.get(node.value, md.TType)[0]

        name = self.get_name(node.targets[0])
        if name in self.env:
            self.constraints += [(val_ty, self.env[name])]

        self.env[name] = val_ty
        return node

    def visit_AugAssign(self, node):
        node.value = self.visit(node.value)
        val_ty = md.get(node.value, md.TType)[0]

        node.target = self.visit(node.target)
        target_ty = md.get(node.target, md.TType)[0]

        self.constraints += [(val_ty, target_ty)]
        return node

    def visit_List(self, node):
        node.elts = map(self.visit, node.elts)
        old_ty = None
        for elt in node.elts:
            elt_ty = md.get(elt, md.TType)[0]
            if old_ty:
                self.constraints += [(old_ty, elt_ty)]
            old_ty = elt_ty
        old_ty = old_ty or self.fresh()
        md.add(node, md.TList(old_ty))
        return node

    def visit_Name(self, node):
        ty = self.env[node.id]
        md.add(node, ty)
        return node

    def visit_Attribute(self, node):
        def rec(a):
            if isinstance(a, ast.Attribute):
                return rec(a.value)[a.attr]
            else:
                assert isinstance(a, ast.Name)
                return MODULES[a.id]
        md.add(node, rec(node).type)
        return node

    def visit_Call(self, node):
        node.func = self.visit(node.func)
        print ast.dump(node)
        f_ty = md.get(node.func, md.TType)[0]
        md.add(node, f_ty.ret)
        return node

    def visit_For(self, node):
        node.iter = self.visit(node.iter)
        iter_ty = md.get(node.iter, md.TType)[0]

        assert isinstance(node.target, ast.Name)
        name = node.target.id
        new_ty = self.fresh()
        if name in self.env:
            self.constraints += [(new_ty, self.env[name])]
        self.env[name] = new_ty

        self.constraints += [(md.TContainer(new_ty), iter_ty)]

        map(self.visit, node.body)
        map(self.visit, node.orelse)

    def visit_Subscript(self, node):
        node.value = self.visit(node.value)
        ty = md.get(node.value, md.TType)[0]
        new_ty = self.fresh()
        md.add(node, new_ty)
        self.constraints += [(md.TContainer(new_ty), ty)]
        return node

    def visit_IfExp(self, node):
        node.test = self.visit(node.test)
        node.body = self.visit(node.body)
        node.orelse = self.visit(node.orelse)
        body_ty = md.get(node.body, md.TType)[0]
        else_ty = md.get(node.orelse, md.TType)[0]
        self.constraints += [(body_ty, else_ty)]
        md.add(node, body_ty)
        return node

    def run(self, node, ctx):
        b = super(TypingTest, self).run(node, ctx)
        for k, v in solve(self.constraints).items():
            print k, " have type : ", v
        print "_____________________________________________________"
        for names, var in self.fun_env.items():
            print "In function : ", names
            for n, t in var.items():
                print "Variable ", n, " have type : ", t
        return b

def unify(c1, c2):
    """
        Unifi les types. Puisque c1 et c2 sont les memes, on retourne un
        dict qui a c1.name associe c2
    """
    if isinstance(c1, md.TFun):
        assert isinstance(c2, md.TFun), "Function match only functions"
        assert len(c1.argtys) == len(c2.argtys)
        assert False, "See later..."
    elif isinstance(c1, (md.TBool, md.TLong)) and isinstance(c2, (md.TBool, md.TLong)):
        return {} # Simple condition...
    elif isinstance(c1, md.TList) and isinstance(c2, md.TList):
        return unify(c1.content_type, c2.content_type)
    elif isinstance(c1, md.TVar):
        return {c1.type_name : c2}
    elif isinstance(c2, md.TVar):
        return {c2.type_name : c1}
    else:
        assert False

def union(s1, s2):
    """
    Fusionne deux dictionnaires
    """
    nenv = s1.copy()
    nenv.update(s2)
    return nenv

def apply(s, t):
    """ s est le nom, t est le type, si le nom correspond, retour un type t
    equivalent mais sans moins complique."""
    if isinstance(t, md.TVar):
        return s.get(t.type_name, t)
    elif isinstance(t, (md.TBool, md.TLong)):
        return t
    elif isinstance(t, md.TList):
        return md.TList(apply(s, t.content_type))
    elif isinstance(t, md.TContainer):
        return md.TContainer(apply(s, t.content_type))
    else:
        assert False, t

def compose(s1, s2):
    s3 = ((t, apply(s1, u)) for t, u in s2.items())
    return union(s1, s3)

def solve(xs):
    mgu = {}
    cs = list(xs)
    while len(cs):
        # Je prend la premiere contrainte
        a, b = cs[-1]
        cs = cs[:-1]
        # Je fais l union des deux types
        s = unify(a, b)
        print s, a, b
        # Je fusionne tous mes anciens types avec le nouveau
        mgu = compose(s, mgu)
# propage le unify a la liste de traitement en cours pour travailler ensuite sur
# des types plus simple. La propagation complete a donc retire un type possible
        cs = [(apply(s, c), apply(s, x)) for c, x in cs]
    return mgu

