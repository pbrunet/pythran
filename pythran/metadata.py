"""
This module provides a way to pass information between passes as metadata.

* add attaches a metadata to a node
* get retrieves all metadata from a particular class attached to a node
"""

from ast import AST  # so that metadata are walkable as regular ast nodes
import string

def naming():
    k = 0
    while True:
        for a in string.ascii_lowercase:
            yield ("'"+a+str(k)) if (k > 0) else (a)
        k = k+1

T_NAMES = naming()

def get_random_name():
    return next(T_NAMES)


class Metadata(AST):
    def __init__(self):
        super(Metadata, self).__init__()
        self.data = list()
        self._fields = ('data',)

    def __iter__(self):
        return iter(self.data)

    def append(self, data):
        self.data.append(data)


class Lazy(AST):

    """ Metadata to mark variable which doesn't need to be evaluated now. """


class Comprehension(AST):
    def __init__(self, *args):  # no positional argument to be deep copyable
        super(Comprehension, self).__init__()
        if args:
            self.target = args[0]

class TType(AST):
    "Type de base"

class TVar(TType):
    def __init__(self, value):
        super(TVar, self).__init__()
        self.type_name = value

    def __str__(self):
        return "TVar(" + self.type_name + ")"

class TLiteral(TVar):
    pass

class TBool(TLiteral):
    pass

class TLong(TLiteral):
    pass

class TFloat(TLiteral):
    pass

class TVaArg(TType):
    def __init__(self, ty=None):
        self.ty = ty

class TIterable(TVar):
    def __init__(self, name, content):
        self.name = name
        self.content_type = content
    def __str__(self):
        return "Iterable(name: " + self.name + ", " + str(self.content_type) + ")"

class TContainer(TIterable):
    def __str__(self):
        return "TContainer(" + str(self.content_type) + ")"

class TDict(TContainer):
    def __init__(self, name, key, value):
        self.key = key
        super(TDict, self).__init__(name, value)

    def __str__(self):
        return "TDict(name:" + self.name + " : " + str(self.key) + ", " + str(self.content_type) + ")"

class TArray(TContainer):
    pass

class TTuple(TContainer):
    def __init__(self, name, *args):
        self.name = name
        self.content_type = list(args)

    def __str__(self):
        return "TTuple(" + str([str(a) for a in self.content_type]) + ", name:" + self.name + ")"

class TNone(TVar):
    def __init__(self):
        pass
    def __str__(self):
        return "TNone"

class TList(TContainer):
    def __str__(self):
        return "TList(" + str(self.content_type) + ", name:" + self.name + ")"

class TFun(TVar):
    def __init__(self, args):
        self.sign = args;

    def __str__(self):
        res = "TFun("
        for sig in self.sign:
            res += str([str(a) for a in sig[1:]])
            res += " -> "
            res += str(sig[0]) + "OR\n"
        return res + ")"

class TModule(TVar):
    def __init__(self, name):
        self.name = name

    def __str__(self):
        return "TModule(" + self.name + ")"

class TIndex(TType):
    "C'est un index"


def replace_vaarg(args_ty, fun_ty):
    """
    args_ty est la liste des arguments utilise,
    fun_ty est le type de la fonction appelle.
    """
    def get_vaarg_len(sign, args):
        """ -1 means no vaarg found. """
        arg_sign = sign[1:]  # Ignore return type for now.
        if len(arg_sign) != len(args_ty):
            return len(args_ty) - len(arg_sign) + 1
        elif any(isinstance(t, TVaArg) for t in arg_sign):
            return 1
        else:
            return -454554#va_arg_len(arg_sign[for a, b in zip(

    def replace(sign, va_arg):
        """ sign est une liste. """
        for i, node in enumerate(sign):
            if isinstance(node, TFun):
                for sub_sign in node.sign:
                    replace(sub_sign, va_arg)
            elif isinstance(node, TVaArg):
                """ Si on a un VaArg, on le remplace par la bonne liste et on enleve le vaarg. """
                assert(i == len(sign) - 1)
                sign.pop()
                if node.ty:
                    sign += [type(node.ty)(next(T_NAMES), arg) for arg in va_arg]
                else:
                    sign += va_arg
                return sign
            elif isinstance(node, TTuple):
                # Si on a un Tuple, il peut contenir un VaArg.
                sign[i] = TTuple(node.name, *[a for a in replace(node.content_type, va_arg)])
            elif isinstance(node, TIterable):
                sign[i] = type(node)(next(T_NAMES), replace([node.content_type], va_arg)[0])
        return sign

    def sign_from_args(sign):
        """
        Use sign and args_ty.
        Return None si la signature ne match pas.
        """
        va_arg_len = get_vaarg_len(sign, args_ty)
        if va_arg_len == -1:
            return None

        va_arg = [TVar(next(T_NAMES)) for _ in xrange(va_arg_len)]

        return replace(sign, va_arg)

    res = list()
    for sign in fun_ty.sign:
        tr_sign = sign_from_args(sign)
        if tr_sign is not None: # Handle empty signature
            res.append(tr_sign)
    return TFun(res)

def add(node, data):
    if not hasattr(node, 'metadata'):
        setattr(node, 'metadata', Metadata())
        node._fields += ('metadata',)
    getattr(node, 'metadata').append(data)


def get(node, class_):
    if hasattr(node, 'metadata'):
        return [s for s in getattr(node, 'metadata') if isinstance(s, class_)]
    else:
        return []


def visit(self, node):
    if hasattr(node, 'metadata'):
        self.visit(node.metadata)
