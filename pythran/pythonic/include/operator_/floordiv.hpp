#ifndef PYTHONIC_INCLUDE_OPERATOR_FLOORDIV_HPP
#define PYTHONIC_INCLUDE_OPERATOR_FLOORDIV_HPP

#include "pythonic/include/utils/proxy.hpp"

namespace pythonic
{

  namespace operator_
  {
    long floordiv(long a, long b);
    double floordiv(double a, long b);
    double floordiv(long a, double b);
    double floordiv(double a, double b);

    PROXY_DECL(pythonic::operator_, floordiv);
  }
}

#endif
