#ifndef PYTHONIC_OPERATOR_IFLOORDIV_HPP
#define PYTHONIC_OPERATOR_IFLOORDIV_HPP

#include "pythonic/include/operator_/ifloordiv.hpp"

#include "pythonic/utils/proxy.hpp"

namespace pythonic
{

  namespace operator_
  {

    template <class A, class B>
    A ifloordiv(A & a, B b)
    {
      return a = std::floor((double)a/b);
    }

    PROXY_IMPL(pythonic::operator_, ifloordiv);
  }
}

#endif
