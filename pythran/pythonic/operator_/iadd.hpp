#ifndef PYTHONIC_OPERATOR_IADD_HPP
#define PYTHONIC_OPERATOR_IADD_HPP

#include "pythonic/include/operator_/iadd.hpp"

#include "pythonic/utils/proxy.hpp"

namespace pythonic
{

  namespace operator_
  {

    namespace proxy
    {

      template <class A, class B>
      auto iadd::operator()(A const &a, B &&b)
          -> decltype(a + std::forward<B>(b))
      {
        return a + std::forward<B>(b);
      }
      template <class A, class B>
      auto iadd::operator()(A &a, B &&b) -> decltype(a += std::forward<B>(b))
      {
        return a += std::forward<B>(b);
      }
    }

    template <class A, class B>
    auto iadd(A &&a, B &&b)
        -> decltype(proxy::iadd{}(std::forward<A>(a), std::forward<B>(b)))
    {
      return proxy::iadd{}(std::forward<A>(a), std::forward<B>(b));
    }
  }
}

#endif
