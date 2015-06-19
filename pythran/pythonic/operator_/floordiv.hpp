#ifndef PYTHONIC_OPERATOR_FLOORDIV_HPP
#define PYTHONIC_OPERATOR_FLOORDIV_HPP

#include "pythonic/include/operator_/floordiv.hpp"

#include "pythonic/utils/proxy.hpp"

namespace pythonic
{

  namespace operator_
  {
    long floordiv(long a, long b)
    {
      return std::floor((double)a / b);
    }
    double floordiv(double a, long b)
    {
      return std::floor(a / b);
    }

    double floordiv(long a, double b)
    {
      return std::floor(a / b);
    }

    double floordiv(double a, double b)
    {
      return std::floor(a / b);
    }

    PROXY_IMPL(pythonic::operator_, floordiv);
  }
}

#endif
