#ifndef ARRAY_BASE
#define ARRAY_BASE

#include "pythonic/include/types/traits.hpp"

namespace pythonic
{
  namespace types
  {
    template <class T>
    using void_t = void;

    template <class, class = void>
    struct has_fast2 : std::false_type {
    };

    template <class T>
    struct has_fast2<T, void_t<typename T::dtype>> : std::true_type {
    };

    template <class T>
    struct ArrayLike {
      static constexpr bool value =
          has_flat_size<T>::value and has_strided<T>::value;
      static_assert(has_dtype<T>::value, "ArrayLike should have a dtype.");
      static_assert(has_vectorizable<T>::value,
                    "ArrayLike should have an is_vectorizable information.");
      static_assert(is_iterable<T>::value, "ArrayLike should be iterable.");
      static_assert(has_shape<T>::value, "ArrayLike should have a shape.");
      // FIXME multiple signature make it harder to detect
      static_assert(has_fast2<T>::value,
                    "ArrayLike should have a fast accessor.");
      // static_assert(has_overlap<T>::value, "ArrayLike should have an
      // may_overlap function.");
    };
  }
}

#endif
