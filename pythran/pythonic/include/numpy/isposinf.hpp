#ifndef PYTHONIC_INCLUDE_NUMPY_ISPOSINF_HPP
#define PYTHONIC_INCLUDE_NUMPY_ISPOSINF_HPP

#include "pythonic/utils/proxy.hpp"
#include"pythonic/types/ndarray.hpp"
#include "pythonic/types/numexpr_to_ndarray.hpp"
#include "pythonic/utils/numpy_traits.hpp"

#include <nt2/include/functions/is_inf.hpp>
#include <nt2/include/functions/is_positive.hpp>

namespace pythonic {

    namespace numpy {
        namespace wrapper {
            template<class T>
                bool isposinf(T const& t);
        }
#define NUMPY_NARY_FUNC_NAME isposinf
#define NUMPY_NARY_FUNC_SYM wrapper::isposinf
#include "pythonic/include/types/numpy_nary_expr.hpp"

    }

}

#endif

