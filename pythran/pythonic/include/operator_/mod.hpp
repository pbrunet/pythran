#ifndef PYTHONIC_INCLUDE_OPERATOR_MOD_HPP
#define PYTHONIC_INCLUDE_OPERATOR_MOD_HPP

#include "pythonic/include/utils/proxy.hpp"
#include "pythonic/include/types/str.hpp"

namespace pythonic
{

  namespace operator_
  {

    template <class A, class B>
    auto mod(A const &a, B const &b) ->
        typename std::enable_if<types::is_fundamental<A>::value and
                                    types::is_fundamental<B>::value,
                                decltype(a % b)>::type;

    inline double mod(double a, long b);

    inline double mod(double a, double b);

    template <class A, class B>
    auto mod(A const &a, B const &b) // for ndarrays
        -> typename std::enable_if<not types::is_fundamental<A>::value or
                                       not types::is_fundamental<B>::value,
                                   decltype(a % b)>::type;

    PROXY_DECL(pythonic::operator_, mod);
  }
}

#endif
