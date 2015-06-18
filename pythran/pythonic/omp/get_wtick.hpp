#ifndef PYTHONIC_OMP_GET_WTICK_HPP
#define PYTHONIC_OMP_GET_WTICK_HPP

#include "pythonic/include/omp/get_wtick.hpp"

#include "pythonic/utils/proxy.hpp"

#include <omp.h>

namespace pythonic
{

  namespace omp
  {
    long get_wtick()
    {
      return omp_get_wtick();
    }

    PROXY_IMPL(pythonic::omp, get_wtick);
  }
}

#endif
