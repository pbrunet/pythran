#ifndef PYTHONIC_INCLUDE_OPERATOR_IADD_HPP
#define PYTHONIC_INCLUDE_OPERATOR_IADD_HPP

#include "pythonic/include/utils/proxy.hpp"

namespace pythonic
{

  namespace operator_
  {

    namespace proxy
    {

      struct iadd {
        template <class A, class B>
        auto operator()(A const &a, B &&b) -> decltype(a + std::forward<B>(b));

        template <class A, class B>
        auto operator()(A &a, B &&b) -> decltype(a += std::forward<B>(b));
      };
    }

    template <class A, class B>
    auto iadd(A &&a, B &&b)
        -> decltype(proxy::iadd{}(std::forward<A>(a), std::forward<B>(b)));
  }
}

#endif
