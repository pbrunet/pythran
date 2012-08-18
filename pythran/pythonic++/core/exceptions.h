#ifndef PYTHONIC_EXCEPTIONS_H
#define PYTHONIC_EXCEPTIONS_H
#include <stdexcept>

namespace pythonic {
   // namespace __exceptions__ {
        class BaseException:public std::exception
        {
                public:
                        BaseException(){}
                        BaseException(std::string value){val = value;}
                        virtual std::string getVal() const{ return val;}
                        virtual ~BaseException() throw(){}
                protected:
                        std::string val;
        };

        class SystemExit:public BaseException{};
        class KeyboardInterrupt:public BaseException{};
        class GeneratorExit:public BaseException{};
        class Exception:public BaseException{};
        class StopIteration:public Exception{};
        class StandardError:public Exception{};
        class Warning:public Exception{};
        class BytesWarning:public Warning{};
        class UnicodeWarning:public Warning{};
        class ImportWarning:public Warning{};
        class FutureWarning:public Warning{};
        class UserWarning:public Warning{};
        class SyntaxWarning:public Warning{};
        class RuntimeWarning:public Warning{};
        class PendingDeprecationWarning:public Warning{};
        class DeprecationWarning:public Warning{};
        class BufferError:public StandardError{};
        class ArithmeticError:public StandardError{};
        class AssertionError:public StandardError{};
        class AttributeError:public StandardError{};
        class EnvironmentError:public StandardError{};
        class EOFError:public StandardError{};
        class ImportError:public StandardError{};
        class LookupError:public StandardError{};
        class MemoryError:public StandardError{};
        class NameError:public StandardError{};
        class ReferenceError:public StandardError{};
        class RuntimeError:public BaseException{};
        class SyntaxError:public StandardError{};
        class SystemError:public StandardError{};
        class TypeError:public StandardError{};
        class ValueError:public StandardError{};
        class FloatingPointError:public ArithmeticError{};
        class OverflowError:public ArithmeticError{};
        class ZeroDivisionError:public ArithmeticError{};
        class IOError:public EnvironmentError{};
        class OSError:public EnvironmentError{};
        class WindowsError:public OSError{};
        class VMSError:public OSError{};
        class IndexError:public LookupError{};
        class KeyError:public LookupError{};
        class UnboundLocalError:public NameError{};
        class NotImplementedError:public RuntimeError{};
        class IndentationError:public SyntaxError{};
        class TabError:public IndentationError{};
        class UnicodeError:public ValueError{};
        class UnicodeDecodeError:public UnicodeError{};
        class UnicodeEncodeError:public UnicodeError{};
        class UnicodeTranslateError:public UnicodeError{};
    //}
}

#endif
