#ifndef PYTHONIC_INCLUDE_OMP_IN_PARALLEL_HPP
#define PYTHONIC_INCLUDE_OMP_IN_PARALLEL_HPP

#include "pythonic/include/utils/proxy.hpp"

namespace pythonic
{

  namespace omp
  {
    bool in_parallel();

    PROXY_DECL(pythonic::omp, in_parallel);
  }
}

#endif
