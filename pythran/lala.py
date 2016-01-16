""" TypingTest replaces builtins by their full paths. """

from pythran.analyses import Globals, Locals
from pythran.passmanager import Transformation
from pythran.syntax import PythranSyntaxError
from pythran.tables import MODULES
from pythran import metadata as md
from pythran.metadata import T_NAMES

import string
import ast


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
        self.names = T_NAMES
        Transformation.__init__(self)

    def fresh(self):
        """ declare a new variable. """
        return md.TVar('$' + next(self.names))

    def visit_Return(self, node):
        if node.value:
            node.value = self.visit(node.value)
            ret_ty = md.get(node.value, md.TType)[0]
            self.constraints += [(ret_ty, self.retty)]
            self.retty = ret_ty
        return node

    def visit_Yield(self, node):
        node.value = self.visit(node.value)
        ret_ty = md.TIterable(next(self.names), md.get(node.value, md.TType)[0])
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
        self.env[node.name] = md.TFun([[self.retty] + self.argtys])
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

    def visit_Dict(self, node):
        keys = map(self.visit, node.keys)
        if keys:
            k_ty = md.get(keys[0], md.TType)[0]
            for k in keys[1:]:
                self.constraints += [(k_ty, md.get(k , md.TType)[0])]
        else:
            k_ty = self.fresh()

        values = map(self.visit, node.values)
        if values:
            v_ty = md.get(values[0], md.TType)[0]
            for v in values[1:]:
                self.constraints += [(v_ty, md.get(v , md.TType)[0])]
        else:
            v_ty = self.fresh()

        md.add(node, md.TDict(self.fresh().type_name, k_ty, v_ty))
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
        md.add(node, md.TList(self.fresh().type_name, old_ty))
        return node

    def visit_Tuple(self, node):
        md.add(node, md.TTuple(self.fresh().type_name, *[md.get(self.visit(elt), md.TType)[0] for elt in node.elts]))
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
        md.add(node, rec(node).type())
        return node

    def visit_Call(self, node):
        node.func = self.visit(node.func)
        node.args = map(self.visit, node.args)
        f_ty = md.get(node.func, md.TType)[0]

        f_ty = md.replace_vaarg(node.args, f_ty)

#        # prise en compte de la back propagation des types
#        for t_arg, r_arg in zip(f_ty.signargs, (md.get(arg, md.TType)[0] for arg in node.args)):
#            self.constraints += [(t_arg, r_arg)]
        t = self.fresh()
        self.constraints += [(md.TFun([[t] + [md.get(arg, md.TType)[0] for arg in node.args]]), f_ty)]

        md.add(node, t)
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

        self.constraints += [(md.TIterable(self.fresh().type_name, new_ty), iter_ty)]

        map(self.visit, node.body)
        map(self.visit, node.orelse)

    def visit_Subscript(self, node):
        node.value = self.visit(node.value)
        ty = md.get(node.value, md.TType)[0]
        new_ty = self.fresh()
        md.add(node, new_ty)
        self.constraints += [(md.TContainer(self.fresh().type_name, new_ty), ty)]
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
        possible_sign = list()
        res = dict()
        for sig1 in c1.sign:
            for sig2 in c2.sign:
                if (len(sig1) == len(sig2) and
                        all(isinstance(a, type(b)) or isinstance(b, type(a))
                            for a, b in zip(sig1, sig2))):
                    possible_sign.append(sig1)
                    for a, b in zip(sig1, sig2):
                        res.update(unify(a, b))
                    return res
        assert False, "no matching signature"
    elif isinstance(c1, (md.TBool, md.TLong)) and isinstance(c2, (md.TBool, md.TLong)):
        return {} # Simple condition...
    elif isinstance(c1, md.TList) and isinstance(c2, md.TList):
        res = unify(c1.content_type, c2.content_type)
        res.update({c1.name: c2})
        return res
    elif isinstance(c1, md.TTuple) and isinstance(c2, md.TTuple):
        res = {}
        for arg1, arg2 in zip(c1.content_type, c2.content_type):
            res.update(unify(arg1, arg2))
        return res
    elif isinstance(c1, md.TDict) and isinstance(c2, md.TDict):
        res = unify(c1.key, c2.key)
        res.update(unify(c1.content_type, c2.content_type))
        res.update({c1.name: c2})
        return res
    elif isinstance(c1, md.TDict) and isinstance(c2, md.TContainer):
        res = unify(c1.content_type, c2.content_type)
        res.update({c2.name: c1})
        return res
    elif isinstance(c1, md.TContainer) and isinstance(c2, md.TDict):
        res = unify(c1.content_type, c2.content_type)
        res.update({c1.name: c2})
        return res
    elif isinstance(c1, md.TContainer) and isinstance(c2, md.TContainer):
        return unify(c1.content_type, c2.content_type)
    elif isinstance(c1, md.TList) and isinstance(c2, md.TIterable):
        res = unify(c1.content_type, c2.content_type)
        res.update({c2.name: c1})
        return res
    elif isinstance(c1, md.TIterable) and isinstance(c2, md.TIterable):
        res = unify(c1.content_type, c2.content_type)
        res.update({c2.name: c1})
        return res
    elif isinstance(c1, md.TNone) and isinstance(c2, md.TNone):
        return dict()
    elif isinstance(c1, md.TVar):
        return {c1.type_name : c2}
    elif isinstance(c2, md.TVar):
        return {c2.type_name : c1}
    else:
        assert False, str(c1) + " " + str(c2)

def union(s1, s2):
    """
    Fusionne deux dictionnaires
    """
    nenv = s1.copy()
    nenv.update(s2)
    return nenv

def apply(s, t):
    """ s est le nom, t est le type, si le nom correspond, retour un type t
    equivalent mais en moins complique."""
    if isinstance(t, (md.TBool, md.TLong, md.TFloat)):
        return t
    elif isinstance(t, md.TList):
        return md.TList(t.name, apply(s, t.content_type))
    elif isinstance(t, md.TDict):
        new_t = s.get(t.name, t)
        new_t.content_type = apply(s, new_t.content_type)
        new_t.key = apply(s, new_t.key)
        return new_t
    elif isinstance(t, md.TTuple):
        return md.TTuple(t.name, *[apply(s, arg) for arg in t.content_type])
    elif isinstance(t, md.TContainer):
        new_t = s.get(t.name, t)
        new_t.content_type = apply(s, new_t.content_type)
        return new_t
    elif isinstance(t, md.TIterable):
        new_t = s.get(t.name, t)
        new_t.content_type = apply(s, new_t.content_type)
        return new_t
    elif isinstance(t, md.TFun):
        for sig in t.sign:
            for i in xrange(len(sig)):
                sig[i] = apply(s, sig[i])
        return t
#        return md.TFun(apply(s, t.ret), *[apply(s, o) for o in t.args])
    elif isinstance(t, md.TNone):
        return t
    elif isinstance(t, md.TVar):
        return s.get(t.type_name, t) or t
    else:
        assert False, (s, t)

def compose(s1, s2):
    s3 = ((t, apply(s1, u)) for t, u in s2.items())
    return union(s1, s3)

def solve(xs):
    mgu = {}
    cs = list(xs)
    while len(cs):
#        print "MGU : ", mgu
#        print ""
        # Je prend la premiere contrainte
        a, b = cs[-1]
        cs = cs[:-1]
        # Je fais l union des deux types
        s = unify(a, b)
#        print "UNIT? : ", s, a, b
        # Je fusionne tous mes anciens types avec le nouveau
        mgu = compose(s, mgu)
# propage le unify a la liste de traitement en cours pour travailler ensuite sur
# des types plus simple. La propagation complete a donc retire un type possible
        cs = [(apply(s, c), apply(s, x)) for c, x in cs]
    return mgu

