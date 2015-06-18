#ifndef PYTHONIC_INCLUDE_BUILTIN_SET_DISCARD_HPP
#define PYTHONIC_INCLUDE_BUILTIN_SET_DISCARD_HPP

#include "pythonic/include/types/none.hpp"
#include "pythonic/include/types/set.hpp"
#include "pythonic/include/utils/proxy.hpp"

namespace pythonic
{

  namespace __builtin__
  {

    namespace set
    {
      template <class T, class U>
      types::none_type discard(types::set<T> &set, U const &elem);

      template <class T, class U>
      types::none_type discard(types::set<T> &&set, U const &elem);

      template <class U>
      types::none_type discard(types::empty_set const &set, U const &elem);

      PROXY_DECL(pythonic::__builtin__::set, discard);
    }
  }
}
#endif
