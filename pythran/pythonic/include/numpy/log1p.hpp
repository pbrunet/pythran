#ifndef PYTHONIC_INCLUDE_NUMPY_LOG1P_HPP
#define PYTHONIC_INCLUDE_NUMPY_LOG1P_HPP

#include "pythonic/utils/proxy.hpp"
#include"pythonic/types/ndarray.hpp"
#include "pythonic/types/numexpr_to_ndarray.hpp"
#include "pythonic/utils/numpy_traits.hpp"
#include <nt2/include/functions/log1p.hpp>

namespace pythonic {

    namespace numpy {
#define NUMPY_NARY_FUNC_NAME log1p
#define NUMPY_NARY_FUNC_SYM nt2::log1p
#include "pythonic/include/types/numpy_nary_expr.hpp"
    }

}

#endif

