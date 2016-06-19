#ifndef PYTHONIC_INCLUDE_TYPES_ASSIGNABLE_HPP
#define PYTHONIC_INCLUDE_TYPES_ASSIGNABLE_HPP

#include <type_traits>

namespace pythonic
{

  struct dummy {
  };

  template <class T>
  struct assignable {
    typedef typename std::remove_cv<typename std::remove_reference<T>::type>::type type;
//    using type = T;
  };

  template <class T>
  struct lazy : assignable<T> {
  }; // very conservative

  template <class T>
  struct returnable : assignable<T> {
  };
}

#endif
