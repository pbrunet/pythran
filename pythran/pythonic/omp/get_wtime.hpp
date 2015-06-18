#ifndef PYTHONIC_OMP_GET_WTIME_HPP
#define PYTHONIC_OMP_GET_WTIME_HPP

#include "pythonic/include/omp/get_wtime.hpp"

#include "pythonic/utils/proxy.hpp"

#include <omp.h>

namespace pythonic
{

  namespace omp
  {

    long get_wtime()
    {
      return omp_get_wtime();
    }

    PROXY_IMPL(pythonic::omp, get_wtime);
  }
}

#endif
