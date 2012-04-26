from test_env import TestEnv

class TestBase(TestEnv):

    def test_pass(self):
        self.run_test("def pass_(a):pass", 1, pass_=("int"))

    def test_empty_return(self):
        self.run_test("def empty_return(a,b,c):return", 1,1,True, empty_return=("int","float","bool"))

    def test_identity(self):
        self.run_test("def identity(a): return a", 1.5, identity=('float'))

    def test_compare(self):
        self.run_test("def compare(a,b,c):\n if a <b < c: return a\n else: return b != c", 1,2,3, compare=("int", "int", "int"))

    def test_arithmetic(self):
        self.run_test("def arithmetic(a,b,c): return a+b*c", 1,2,3.3, arithmetic=('int','int', 'float'))

    def test_boolop(self):
        self.run_test("def boolop(a,b,c): return a and b or c", True, True, False, boolop=('bool','bool', 'bool'))

    def test_operator(self):
        self.run_test("def operator_(a,b,c): return (a+b-b*a/(a%b)**(a<<a>>b|b^a&a/b)//c)",1,2,1.5, operator_=('int','int', 'float'))

    def test_unaryop(self):
        self.run_test("def unaryop(a): return not(~(+(-a)))", 1, unaryop=("int"))

    def test_expression(self):
        self.run_test("def expression(a,b,c): a+b*c", 1,2,3.3, expression=('int','int', 'float'))

    def test_recursion(self):
        code="""
def fibo(n): return n if n <2 else fibo(n-1) + fibo(n-2)
def fibo2(n): return fibo2(n-1) + fibo2(n-2) if n > 1 else n
"""
        self.run_test(code, 4, fibo=('int'), fibo2=('float'))

    def test_list_comprehension(self):
        self.run_test("def list_comprehension(l): return [ x*x for x in l ]", [1,2,3], list_comprehension=("int list"))

    def test_filtered_list_comprehension(self):
        self.run_test("def filtered_list_comprehension(l): return [ x*x for x in l if x > 1 if x <10]", [1,2,3], filtered_list_comprehension=("int list"))

    def test_multilist_comprehension(self):
        self.run_test("def multilist_comprehension(l): return [ x*y for x in l for y in l]", [1,2,3], multilist_comprehension=("int list"))

    def test_zipped_list_comprehension(self):
        self.run_test("def zipped_list_comprehension(l): return [ x*y for x,y in zip(l,l) ]", [1,2,3], zipped_list_comprehension=("int list"))

    def test_zip(self):
        self.run_test("def zip_(l0,l1): return zip(l0,l1)", [1,2,3],["one", "two", "three"], zip_=("int list", "str list"))

    def test_multizip(self):
        self.run_test("def multizip(l0,l1): return zip(l0,zip(l0,l1))", [1,2,3],["one", "two", "three"], multizip=("int list", "str list"))

    def test_reduce(self):
        self.run_test("def reduce_(l): return reduce(lambda x,y:x+y, l)", [0,1.1,2.2,3.3], reduce_=("float list"))

    def test_sum(self):
        self.run_test("def sum_(l): return sum( l)", [0,1.1,2.2,3.3], sum_=("float list"))

    def test_multisum(self):
        self.run_test("def multisum(l0,l1): return sum(l0)+sum(l1)", [0,1.1,2.2,3.3],[True,False,True], multisum=("float list","bool list"))

    def test_max(self):
        self.run_test("def max_(l):return max(l)", [ 1.1, 2.2 ], max_=("float list"))

    def test_multimax(self):
        self.run_test("def multimax(l,v):return max(v,max(l))", [ 1.1, 2.2 ], 3, multimax=("float list","int"))

    def test_min(self):
        self.run_test("def min_(l):return min(l)", [ 1.1, 2.2 ], min_=("float list"))

    def test_multimin(self):
        self.run_test("def multimin(l,v):return min(v,min(l))", [ 1.1, 2.2 ], 3, multimin=("float list","int"))

    def test_map(self):
        self.run_test("def map_(l0, l1,v): return map(lambda x,y:x*v+y, l0, l1)", [0,1,2], [0,1.1,2.2], 2, map_=("int list", "float list", "int"))
    
    def test_multimap(self):
        self.run_test("def multimap(l0, l1,v): return map(lambda x,y:x*v+y, l0, map(lambda z:z+1,l1))", [0,1,2], [0,1.1,2.2], 2, multimap=("int list", "float list", "int"))

    def test_range1(self):
        self.run_test("def range1_(e): return range(e)", 3, range1_=("int"))
    
    def test_range2(self):
        self.run_test("def range2_(b,e): return range(b,e)", 1, 3, range2_=("int","int"))
    
    def test_range3(self):
        self.run_test("def range3_(b,e,s): return range(b,e,s)", 8,3,-2, range3_=("int","int","int"))
    
    def test_range4(self):
        self.run_test("def range4_(b,e,s): return range(b,e,s)", 3,8,1, range4_=("int","int","int"))

    def test_multirange(self):
        self.run_test("def multirange(i): return map(lambda x,y:y*x/2, range(1,i), range(i,1,-1))", 3, multirange=("int"))
    
    def test_xrange1(self):
        self.run_test("def xrange1_(e): return xrange(e)", 3, xrange1_=("int"))
    
    def test_xrange2(self):
        self.run_test("def xrange2_(b,e): return xrange(b,e)", 1, 3, xrange2_=("int","int"))
    
    def test_xrange3(self):
        self.run_test("def xrange3_(b,e,s): return xrange(b,e,s)", 8,3,-2, xrange3_=("int","int","int"))
    
    def test_xrange4(self):
        self.run_test("def xrange4_(b,e,s): return xrange(b,e,s)", 3,8,1, xrange4_=("int","int","int"))

    def test_multixrange(self):
        self.run_test("def multixrange(i): return map(lambda x,y:y*x/2, xrange(1,i), xrange(i,1,-1))", 3, multixrange=("int"))
    
    def test_print(self):
        self.run_test("def print_(a,b,c,d): print a,b,c,d,'e',1.5,", [1,2,3.1],3,True, "d", print_=("float list", "int", "bool", "str"))

    def test_sequence(self):
        self.run_test("def sequence_(a): b=2*a;c=b/2;return max(c,b)", 1, sequence_=("int"))

    def test_if(self):
        self.run_test("def if_(a,b):\n if a>b: return a\n else: return b", 1, 1.1, if_=("int", "float"))

    def test_while(self):
        self.run_test("def while_(a):\n while(a>0): a-=1\n else: a=0\n return a", 8, while_=("int"))

    def test_for(self):
        self.run_test("def for_(l):\n s=0\n for i in l:\n  s+=i\n return s", [0,1,2], for_=("float list"))

    def test_declarations(self):
        code = """
def declarations():
    if True:
        a=0
        while a <3:
            b = 1
            a = b + a
    else:
        a=1
    return a + b
"""
        self.run_test(code, declarations=())
    def test_lambda(self):
        code = """
def lambda_():
    l=lambda x,y: x+y
    return l(1,2) + l(1.2,2)
"""
        self.run_test(code, lambda_=())

    def test_multidef1(self):
        self.run_test("def def10(): pass\ndef def11(): def10()", def11=())

    def test_multidef2(self):
        self.run_test("def def21(): def20()\ndef def20(): pass", def21=())
        
    def test_multidef3(self):
        self.run_test("def def31(): return 1\ndef def30(): return def31()", def31=())
        
    def test_multidef4(self):
       self.run_test("def def41(): return def40()\ndef def40(): return 1", def41=())
        
    def test_tuple(self):
        self.run_test("def tuple_(t): return t[0]+t[1]", (0,1), tuple_=("int int tuple"))

    def test_nested_list_comprehension(self):
        self.run_test("def nested_list_comprehension(): return [ [ 0 for x in xrange(10) ] for y in xrange(20) ]", nested_list_comprehension=())

    def test_delete(self):
        self.run_test("def delete_(v): del v", 1, delete_=("int"))

    def test_continue(self):
        self.run_test("def continue_():\n for i in xrange(3):continue", continue_=())

    def test_break(self):
        self.run_test("def break_():\n for i in xrange(3):break", break_=())

    def test_assert(self):
        self.run_test("def assert_(i): assert i > 0", 1, assert_=("int"))

    def test_assert_with_msg(self):
        self.run_test("def assert_with_msg(i): assert i > 0, 'hell yeah'", 1, assert_with_msg=("int"))

    def test_import_from(self):
        self.run_test("def import_from(): from math import cos", import_from=())

    def test_len(self):
        self.run_test("def len_(i,j,k): return len(i)+len(j)+len(k)", "youpi", [1,2],[], len_=("str","int list", "float list"))

    def test_in_string(self):
        self.run_test("def in_string(i,j): return i in j", "yo", "youpi", in_string=("str","str"))

    def test_not_in_string(self):
        self.run_test("def not_in_string(i,j): return i not in j", "yo", "youpi", not_in_string=("str","str"))

    def test_in_sequence(self):
        self.run_test("def in_sequence(i,j): return i in j", 1, [1,2,3], in_sequence=("int","int list"))

    def test_not_in_sequence(self):
        self.run_test("def not_in_sequence(i,j): return i not in j", False, [True, True, True], not_in_sequence=("bool","bool list"))

    def test_subscript(self):
        self.run_test("def subscript(l,i): l[0]=l[0]+l[i]", [1], 0, subscript=("int list", "int"))

    def test_nested_sequences(self):
        self.run_test("def nested_sequences(l,i): return l[0][i]", [[1]], 0, nested_sequences=("int list list","int"))

    def test_nested_tuples(self):
        self.run_test("def nested_tuples(l,i): return l[i][1]", [(0.1,1,)], 0, nested_tuples=("float int tuple list ","int"))

    def test_return_empty_list(self):
        self.run_test("def return_empty_list(): return list()", return_empty_list=())

    def test_empty_list(self):
        self.run_test("def empty_list(): a=[]", empty_list=())

    def test_multi_list(self):
        self.run_test("def multi_list(): return [[[2.0],[1,2,3]],[[2.0],[1,2,3]]]", multi_list=())

    def test_empty_tuple(self):
        self.run_test("def empty_tuple(): a=()", empty_tuple=())

    def test_multi_tuple(self):
        self.run_test("def multi_tuple(): return (1,('e',2.0),[1,2,3])", multi_tuple=())

    def test_augmented_assign(self):
        self.run_test("def augmented_assign():\n a=0\n a+=1.5\n return a", augmented_assign=())

    def test_augmented_list_assign(self):
        self.run_test("def augmented_list_assign(l):\n a=list()\n a+=l\n return a", [1,2], augmented_list_assign=("int list"))

    def test_initialization_list(self):
        self.run_test("def initialization_list(): return [1, 2.3]", initialization_list=())

    def test_multiple_assign(self):
        self.run_test("def multiple_assign():\n a=0\n a=1.5\n return a", multiple_assign=())
    def test_multiple_return1(self):
        self.run_test("def multiple_return1(a):\n if True:return 1\n else:\n  return a", 2,  multiple_return1=("int"))

    def test_multiple_return2(self):
        self.run_test("def multiple_return2(a):\n if True:return 1\n else:\n  b=a\n  return b", 2,  multiple_return2=("int"))

    def test_multiple_return3(self):
        self.run_test("def multiple_return3(a):\n if True:return 1\n else:\n  b=a\n  return a", 2,  multiple_return3=("int"))

    def test_id(self):
        self.run_test("def id_(a,b):\n c=a\n d=b\n return id(a)==id(c), id(b)==id(d)", [1,2,3], "ert", id_=("int list", "str"))

    def test_delayed_max(self):
        self.run_test("def delayed_max(a,b,c):\n m=max\n return m(a,b) + m(b,c)", 1, 2, 3.5, delayed_max=("int", "int", "float"))
