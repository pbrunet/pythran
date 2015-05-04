#ifndef PYTHONIC_INCLUDE_NUMPY_INT64_HPP
#define PYTHONIC_INCLUDE_NUMPY_INT64_HPP

#include "pythonic/utils/proxy.hpp"
#include "pythonic/utils/meta.hpp"
#include "pythonic/utils/numpy_traits.hpp"
#include "pythonic/types/numpy_op_helper.hpp"

namespace pythonic {

    namespace numpy {

        namespace details {

            int64_t int64();
            template<class V>
                int64_t int64(V v);
        }

#define NUMPY_NARY_FUNC_NAME int64
#define NUMPY_NARY_FUNC_SYM details::int64
#include "pythonic/include/types/numpy_nary_expr.hpp"

    }

}

#endif

