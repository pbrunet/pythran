#ifndef PYTHONIC_INCLUDE_OPERATOR_ICONCAT_HPP
#define PYTHONIC_INCLUDE_OPERATOR_ICONCAT_HPP

#include "pythonic/include/utils/proxy.hpp"

namespace pythonic
{

  namespace operator_
  {
    template <class A, class B>
    A iconcat(A a, B const &b);

    PROXY_DECL(pythonic::operator_, iconcat);
  }
}

#endif
