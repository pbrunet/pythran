#ifndef PYTHONIC_OMP_SET_NESTED_HPP
#define PYTHONIC_OMP_SET_NESTED_HPP

#include "pythonic/include/omp/set_nested.hpp"

#include "pythonic/utils/proxy.hpp"

#include <omp.h>

namespace pythonic
{

  namespace omp
  {

    void set_nested(long val)
    {
      return omp_set_nested(val);
    }

    PROXY_IMPL(pythonic::omp, set_nested);
  }
}

#endif
