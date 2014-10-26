""" Common function use for AST manipulation. """

import ast


def path_to_attr(path):
    """
    Transform path to ast.Attribute.

    >> import ast
    >> path = ('__builtin__', 'my', 'constant')
    >> print ast.dump(path_to_attr(path))
    ast.Attribute(value=ast.Attribute(value=ast.Name(id="__builtin__",
    ..                                               ctx=ast.Load()),
    ..                                attr="my", ctx=ast.Load()),
    ..            attr="constant", ctx=ast.Load())
    """
    return reduce(lambda hpath, last: ast.Attribute(hpath, last, ast.Load()),
                  path[1:], ast.Name(path[0], ast.Load()))
