#ifndef PYTHONIC_OPERATOR_FLOORDIV_HPP
#define PYTHONIC_OPERATOR_FLOORDIV_HPP

#include "pythonic/include/operator_/floordiv.hpp"

#include "pythonic/utils/proxy.hpp"
#include <nt2/include/functions/divfloor.hpp>
#include <gmpxx.h>

namespace pythonic
{

  namespace operator_
  {

    template <class T, class U>
      typename std::enable_if<types::is_integral<T>::value and types::is_integral<U>::value, long>::type
    floordiv(T a, U b)
    {
      return std::floor(static_cast<double>(a) / b);
    }

    template <class T, class U, class A, class B>
      typename std::enable_if<std::is_same<U, __gmp_expr<A, B>>::value, long>::type
    floordiv(T a, U b)
    {
      return std::floor(a / b.get_d());
    }

    template <class T, class U, class A, class B>
      typename std::enable_if<std::is_same<T, __gmp_expr<A, B>>::value, long>::type
    floordiv(T a, U b)
    {
      return std::floor(a.get_d() / b);
    }

    template <class T, class U, class A, class B>
      typename std::enable_if<not (types::is_integral<T>::value and types::is_integral<U>::value) and not std::is_same<U, __gmp_expr<A, B>>::value and not std::is_same<T, __gmp_expr<A, B>>::value, double>::type
    floordiv(T a, U b)
    {
      return std::floor(a / b);
    }



    PROXY_IMPL(pythonic::operator_, floordiv);
  }
}

#endif
