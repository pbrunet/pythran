#ifndef PYTHONIC_EXCEPTIONS_H
#define PYTHONIC_EXCEPTIONS_H
#include <stdexcept>
#include <string>

namespace pythonic {
    namespace core {
        class BaseException: public std::exception
        {
            public:
                    BaseException(const BaseException &e){args = e.args;}
                    template<typename ... Types>
                    BaseException(std::string first,Types ... types){init(first, types ...);}
                    virtual core::list<std::string> getArgs() const{ return core::list<std::string>(args.begin(), args.end());} //not perfect but how to do in another way? conversion from list to tuple may need to be improve...
                    virtual ~BaseException() throw(){}
            protected:
                    template<typename T, typename ... Types>
                    void init(T& first, Types ... others){ args.push_back(first); init(others ...);}
                    template<typename T>
                    void init(T& first){args.push_back(first);}
                    std::vector<std::string> args;
        };
        class SystemExit:public BaseException
        {
            public:
                SystemExit(const SystemExit& e):BaseException(e){}
                template<class ... Types>
                SystemExit(std::string first, Types ... types):BaseException(first, types ...){}
                virtual ~SystemExit() throw(){}
        };
        class KeyboardInterrupt:public BaseException
        {
            public:
                KeyboardInterrupt(const KeyboardInterrupt& e):BaseException(e){}
                template<class ... Types>
                KeyboardInterrupt(std::string first, Types ... types):BaseException(first, types ...){}
                virtual ~KeyboardInterrupt() throw(){}
        };
        class GeneratorExit:public BaseException
        {
            public:
                GeneratorExit(const GeneratorExit& e):BaseException(e){}
                template<class ... Types>
                GeneratorExit(std::string first, Types ... types):BaseException(first, types ...){}
                virtual ~GeneratorExit() throw(){}
        };
        class Exception:public BaseException
        {
            public:
                Exception(const Exception& e):BaseException(e){}
                template<class ... Types>
                Exception(std::string first,Types ... types):BaseException(first, types ...){}
                virtual ~Exception() throw(){}
        };
        class StopIteration:public Exception
        {
            public:
                StopIteration(const StopIteration& e):Exception(e){}
                template<class ... Types>
                StopIteration(std::string first, Types ... types):Exception(first, types ...){}
                virtual ~StopIteration() throw(){}
        };
        class StandardError:public Exception
        {
            public:
                StandardError(const StandardError& e):Exception(e){}
                template<class ... Types>
                StandardError(std::string first, Types ... types):Exception(first, types ...){}
                virtual ~StandardError() throw(){}
        };
        class Warning:public Exception
        {
            public:
                Warning(const Warning& e):Exception(e){}
                template<class ... Types>
                Warning(std::string first, Types ... types):Exception(first, types ...){}
                virtual ~Warning() throw(){}
        };
        class BytesWarning:public Warning
        {
            public:
                BytesWarning(const BytesWarning& e):Warning(e){}
                template<class ... Types>
                BytesWarning(std::string first, Types ... types):Warning(first, types ...){}
                virtual ~BytesWarning() throw(){}
        };
        class UnicodeWarning:public Warning
        {
            public:
                UnicodeWarning(const UnicodeWarning& e):Warning(e){}
                template<class ... Types>
                UnicodeWarning(std::string first, Types ... types):Warning(first, types ...){}
                virtual ~UnicodeWarning() throw(){}
        };
        class ImportWarning:public Warning
        {
            public:
                ImportWarning(const ImportWarning& e):Warning(e){}
                template<class ... Types>
                ImportWarning(std::string first, Types ... types):Warning(first, types ...){}
                virtual ~ImportWarning() throw(){}
        };
        class FutureWarning:public Warning
        {
            public:
                FutureWarning(const FutureWarning& e):Warning(e){}
                template<class ... Types>
                FutureWarning(std::string first, Types ... types):Warning(first, types ...){}
                virtual ~FutureWarning() throw(){}
        };
        class UserWarning:public Warning
        {
            public:
                UserWarning(const UserWarning& e):Warning(e){}
                template<class ... Types>
                UserWarning(std::string first, Types ... types):Warning(first, types ...){}
                virtual ~UserWarning() throw(){}
        };
        class SyntaxWarning:public Warning
        {
            public:
                SyntaxWarning(const SyntaxWarning& e):Warning(e){}
                template<class ... Types>
                SyntaxWarning(std::string first, Types ... types):Warning(first, types ...){}
                virtual ~SyntaxWarning() throw(){}
        };
        class RuntimeWarning:public Warning
        {
            public:
                RuntimeWarning(const RuntimeWarning& e):Warning(e){}
                template<class ... Types>
                RuntimeWarning(std::string first, Types ... types):Warning(first, types ...){}
                virtual ~RuntimeWarning() throw(){}
        };
        class PendingDeprecationWarning:public Warning
        {
            public:
                PendingDeprecationWarning(const PendingDeprecationWarning& e):Warning(e){}
                template<class ... Types>
                PendingDeprecationWarning(std::string first, Types ... types):Warning(first, types ...){}
                virtual ~PendingDeprecationWarning() throw(){}
        };
        class DeprecationWarning:public Warning
        {
            public:
                DeprecationWarning(const DeprecationWarning& e):Warning(e){}
                template<class ... Types>
                DeprecationWarning(std::string first, Types ... types):Warning(first, types ...){}
                virtual ~DeprecationWarning() throw(){}
        };
        class BufferError:public StandardError
        {
            public:
                BufferError(const BufferError& e):StandardError(e){}
                template<class ... Types>
                BufferError(std::string first, Types ... types):StandardError(first, types ...){}
                virtual ~BufferError() throw(){}
        };
        class ArithmeticError:public StandardError
        {
            public:
                ArithmeticError(const ArithmeticError& e):StandardError(e){}
                template<class ... Types>
                ArithmeticError(std::string first, Types ... types):StandardError(first, types ...){}
                virtual ~ArithmeticError() throw(){}
        };
        class AssertionError:public StandardError
        {
            public:
                AssertionError(const AssertionError& e):StandardError(e){}
                template<class ... Types>
                AssertionError(std::string first, Types ... types):StandardError(first, types ...){}
                virtual ~AssertionError() throw(){}
        };
        class AttributeError:public StandardError
        {
            public:
                AttributeError(const AttributeError& e):StandardError(e){}
                template<class ... Types>
                AttributeError(std::string first, Types ... types):StandardError(first, types ...){}
                virtual ~AttributeError() throw(){}
        };
        class EnvironmentError:public StandardError
        {
            public:
                EnvironmentError(const EnvironmentError& e):StandardError(e){}
                template<class ... Types>
                EnvironmentError(std::string first, Types ... types):StandardError(first, types ...){}
                virtual ~EnvironmentError() throw(){}
        };
        class EOFError:public StandardError
        {
            public:
                EOFError(const EOFError& e):StandardError(e){}
                template<class ... Types>
                EOFError(std::string first, Types ... types):StandardError(first, types ...){}
                virtual ~EOFError() throw(){}
        };
        class ImportError:public StandardError
        {
            public:
                ImportError(const ImportError& e):StandardError(e){}
                template<class ... Types>
                ImportError(std::string first, Types ... types):StandardError(first, types ...){}
                virtual ~ImportError() throw(){}
        };
        class LookupError:public StandardError
        {
            public:
                LookupError(const LookupError& e):StandardError(e){}
                template<class ... Types>
                LookupError(std::string first, Types ... types):StandardError(first, types ...){}
                virtual ~LookupError() throw(){}
        };
        class MemoryError:public StandardError
        {
            public:
                MemoryError(const MemoryError& e):StandardError(e){}
                template<class ... Types>
                MemoryError(std::string first, Types ... types):StandardError(first, types ...){}
                virtual ~MemoryError() throw(){}
        };
        class NameError:public StandardError
        {
            public:
                NameError(const NameError& e):StandardError(e){}
                template<class ... Types>
                NameError(std::string first, Types ... types):StandardError(first, types ...){}
                virtual ~NameError() throw(){}
        };
        class ReferenceError:public StandardError
        {
            public:
                ReferenceError(const ReferenceError& e):StandardError(e){}
                template<class ... Types>
                ReferenceError(std::string first, Types ... types):StandardError(first, types ...){}
                virtual ~ReferenceError() throw(){}
        };
        class RuntimeError:public StandardError
        {
            public:
                RuntimeError(const RuntimeError& e):StandardError(e){}
                template<class ... Types>
                RuntimeError(std::string first, Types ... types):StandardError(first, types ...){}
                virtual ~RuntimeError() throw(){}
        };
        class SyntaxError:public StandardError
        {
            public:
                SyntaxError(const SyntaxError& e):StandardError(e){}
                template<class ... Types>
                SyntaxError(std::string first, Types ... types):StandardError(first, types ...){}
                virtual ~SyntaxError() throw(){}
        };
        class SystemError:public StandardError
        {
            public:
                SystemError(const SystemError& e):StandardError(e){}
                template<class ... Types>
                SystemError(std::string first, Types ... types):StandardError(first, types ...){}
                virtual ~SystemError() throw(){}
        };
        class TypeError:public StandardError
        {
            public:
                TypeError(const TypeError& e):StandardError(e){}
                template<class ... Types>
                TypeError(std::string first, Types ... types):StandardError(first, types ...){}
                virtual ~TypeError() throw(){}
        };
        class ValueError:public StandardError
        {
            public:
                ValueError(const ValueError& e):StandardError(e){}
                template<class ... Types>
                ValueError(std::string first, Types ... types):StandardError(first, types ...){}
                virtual ~ValueError() throw(){}
        };
        class FloatingPointError:public ArithmeticError
        {
            public:
                FloatingPointError(const FloatingPointError& e):ArithmeticError(e){}
                template<class ... Types>
                FloatingPointError(std::string first, Types ... types):ArithmeticError(first, types ...){}
                virtual ~FloatingPointError() throw(){}
        };
        class OverflowError:public ArithmeticError
        {
            public:
                OverflowError(const OverflowError& e):ArithmeticError(e){}
                template<class ... Types>
                OverflowError(std::string first, Types ... types):ArithmeticError(first, types ...){}
                virtual ~OverflowError() throw(){}
        };
        class ZeroDivisionError:public ArithmeticError
        {
            public:
                ZeroDivisionError(const ZeroDivisionError& e):ArithmeticError(e){}
                template<class ... Types>
                ZeroDivisionError(std::string first, Types ... types):ArithmeticError(first, types ...){}
                virtual ~ZeroDivisionError() throw(){}
        };
        class IOError:public EnvironmentError
        {
            public:
                IOError(const IOError& e):EnvironmentError(e){}
                template<class ... Types>
                IOError(std::string first, Types ... types):EnvironmentError(first, types ...){}
                virtual ~IOError() throw(){}
        };
        class OSError:public EnvironmentError
        {
            public:
                OSError(const OSError& e):EnvironmentError(e){}
                template<class ... Types>
                OSError(std::string first, Types ... types):EnvironmentError(first, types ...){}
                virtual ~OSError() throw(){}
        };
        class WindowsError:public OSError
        {
            public:
                WindowsError(const WindowsError& e):OSError(e){}
                template<class ... Types>
                WindowsError(std::string first, Types ... types):OSError(first, types ...){}
                virtual ~WindowsError() throw(){}
        };
        class VMSError:public OSError
        {
            public:
                VMSError(const VMSError& e):OSError(e){}
                template<class ... Types>
                VMSError(std::string first, Types ... types):OSError(first, types ...){}
                virtual ~VMSError() throw(){}
        };
        class IndexError:public LookupError
        {
            public:
                IndexError(const IndexError& e):LookupError(e){}
                template<class ... Types>
                IndexError(std::string first, Types ... types):LookupError(first, types ...){}
                virtual ~IndexError() throw(){}
        };
        class KeyError:public LookupError
        {
            public:
                KeyError(const KeyError& e):LookupError(e){}
                template<class ... Types>
                KeyError(std::string first, Types ... types):LookupError(first, types ...){}
                virtual ~KeyError() throw(){}
        };
        class UnboundLocalError:public NameError
        {
            public:
                UnboundLocalError(const UnboundLocalError& e):NameError(e){}
                template<class ... Types>
                UnboundLocalError(std::string first, Types ... types):NameError(first, types ...){}
                virtual ~UnboundLocalError() throw(){}
        };
        class NotImplementedError:public RuntimeError
        {
            public:
                NotImplementedError(const NotImplementedError& e):RuntimeError(e){}
                template<class ... Types>
                NotImplementedError(std::string first, Types ... types):RuntimeError(first, types ...){}
                virtual ~NotImplementedError() throw(){}
        };
        class IndentationError:public SyntaxError
        {
            public:
                IndentationError(const IndentationError& e):SyntaxError(e){}
                template<class ... Types>
                IndentationError(std::string first, Types ... types):SyntaxError(first, types ...){}
                virtual ~IndentationError() throw(){}
        };
        class TabError:public IndentationError
        {
            public:
                TabError(const TabError& e):IndentationError(e){}
                template<class ... Types>
                TabError(std::string first, Types ... types):IndentationError(first, types ...){}
                virtual ~TabError() throw(){}
        };
        class UnicodeError:public ValueError
        {
            public:
                UnicodeError(const UnicodeError& e):ValueError(e){}
                template<class ... Types>
                UnicodeError(std::string first, Types ... types):ValueError(first, types ...){}
                virtual ~UnicodeError() throw(){}
        };
        class UnicodeDecodeError:public UnicodeError
        {
            public:
                UnicodeDecodeError(const UnicodeDecodeError& e):UnicodeError(e){}
                template<class ... Types>
                UnicodeDecodeError(std::string first, Types ... types):UnicodeError(first, types ...){}
                virtual ~UnicodeDecodeError() throw(){}
        };
        class UnicodeEncodeError:public UnicodeError
        {
            public:
                UnicodeEncodeError(const UnicodeEncodeError& e):UnicodeError(e){}
                template<class ... Types>
                UnicodeEncodeError(std::string first, Types ... types):UnicodeError(first, types ...){}
                virtual ~UnicodeEncodeError() throw(){}
        };
        class UnicodeTranslateError:public UnicodeError
        {
            public:
                UnicodeTranslateError(const UnicodeTranslateError& e):UnicodeError(e){}
                template<class ... Types>
                UnicodeTranslateError(std::string first, Types ... types):UnicodeError(first, types ...){}
                virtual ~UnicodeTranslateError() throw(){}
        };
    }
}

#endif
