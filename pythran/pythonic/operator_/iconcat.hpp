#ifndef PYTHONIC_OPERATOR_ICONCAT_HPP
#define PYTHONIC_OPERATOR_ICONCAT_HPP

#include "pythonic/include/operator_/iconcat.hpp"

#include "pythonic/utils/proxy.hpp"

namespace pythonic
{

  namespace operator_
  {
    template <class A, class B>
    A iconcat(A a, B const &b)
    {
      return a += b;
    }

    PROXY_IMPL(pythonic::operator_, iconcat);
  }
}

#endif
