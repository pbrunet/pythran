#ifndef PYTHONIC_MATH_MODF_HPP
#define PYTHONIC_MATH_MODF_HPP

#include "pythonic/include/math/modf.hpp"

#include "pythonic/utils/proxy.hpp"
#include "pythonic/types/tuple.hpp"

#include <cmath>

namespace pythonic
{

  namespace math
  {

    types::array<double, 2> modf(double x)
    {
      double i;
      double frac = std::modf(x, &i);
      return types::make_tuple(frac, i);
    }

    PROXY_IMPL(pythonic::math, modf);
  }
}

#endif
