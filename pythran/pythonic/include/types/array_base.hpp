#ifndef ARRAY_BASE
#define ARRAY_BASE

#include "pythonic/include/types/traits.hpp"

namespace pythonic
{
  namespace types
  {

    template <class T>
    class ArrayLike
    {
      static_assert(has_size<T>::value,
                    "ArrayLike should have a size function.");
    };
  }
}

#endif
