#ifndef PYTHONIC_BUILTIN_SET_DISCARD_HPP
#define PYTHONIC_BUILTIN_SET_DISCARD_HPP

#include "pythonic/include/__builtin__/set/discard.hpp"

#include "pythonic/__builtin__/None.hpp"
#include "pythonic/types/none.hpp"
#include "pythonic/types/set.hpp"
#include "pythonic/utils/proxy.hpp"

namespace pythonic
{

  namespace __builtin__
  {

    namespace set
    {
      template <class T, class U>
      types::none_type discard(types::set<T> &set, U const &elem)
      {
        set.discard(elem);
        return __builtin__::None;
      }

      template <class T, class U>
      types::none_type discard(types::set<T> &&set, U const &elem)
      {
        // nothing to be done for lvalue
        return __builtin__::None;
      }

      template <class U>
      types::none_type discard(types::empty_set const &set, U const &elem)
      {
        // nothing to remove in an empty_set
        return __builtin__::None;
      }

      PROXY_IMPL(pythonic::__builtin__::set, discard);
    }
  }
}
#endif
