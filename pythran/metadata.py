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

class TBool(TType):
    pass

class TLong(TType):
    pass

class TFloat(TType):
    pass

class TVaArg(TType):
    def __init__(self, ty=None):
        self.ty = ty

class TContainer(TType):
    def __init__(self, content):
        self.content_type = content

    def __str__(self):
        return "TContainer(" + str(self.content_type) + ")"

class TArray(TContainer):
    pass

class TTuple(TContainer):
    pass

class TNone(TType):
    pass

class TList(TContainer):
    def __str__(self):
        return "TList(" + str(self.content_type) + ")"

class TVar(TType):
    def __init__(self, value):
        super(TVar, self).__init__()
        self.type_name = value

    def __str__(self):
        return "TVar(" + self.type_name + ")"

class TFun(TType):
    def __init__(self, ret, *args):
        self.ret = ret
        self.args = args

    def __str__(self):
        return "TFun(" + str([str(a) for a in self.args]) + " -> " + str(self.ret) + ")"

class TModule(TType):
    def __init__(self, name):
        self.name = name

class TIndex(TType):
    "C'est un index"


def replace_vaarg(num, t):
    va_arg = list()
    for _ in xrange(num):
        va_arg.append(TVar(naming().next() + "___"))
    assert isinstance(t, TFun), t
    def rec(ty):
        if isinstance(ty, TFun):
            ty.ret = rec(ty.ret)
            args = list()
            for arg in ty.args:
                if isinstance(arg, TVaArg):
                    if arg.ty:
                        args += map(type(arg.ty), va_arg)
                    else:
                        args += va_arg
                else:
                    args += [rec(arg)]
            ty.args = args
            return ty
        elif isinstance(ty, TVaArg):
            return va_arg if not ty.ty else map(type(ty.ty), va_arg)
        elif isinstance(ty, TContainer):
            return type(ty)(rec(ty.content_type))
        else:
            return ty
    return rec(t)

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
