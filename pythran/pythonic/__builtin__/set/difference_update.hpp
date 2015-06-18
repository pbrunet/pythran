#ifndef PYTHONIC_BUILTIN_SET_DIFFERENCEUPDATE_HPP
#define PYTHONIC_BUILTIN_SET_DIFFERENCEUPDATE_HPP

#include "pythonic/include/__builtin__/set/difference_update.hpp"

#include "pythonic/types/set.hpp"
#include "pythonic/types/none.hpp"
#include "pythonic/__builtin__/None.hpp"
#include "pythonic/utils/proxy.hpp"

namespace pythonic
{

  namespace __builtin__
  {

    namespace set
    {

      template <typename T, typename... Types>
      types::none_type difference_update(types::set<T> &set,
                                         Types const &... others)
      {
        set.difference_update(others...);
        return __builtin__::None;
      }

      template <typename T, typename... Types>
      types::none_type difference_update(types::set<T> &&set,
                                         Types const &... others)
      {
        // nothing to be done as we work on rvalue
        return __builtin__::None;
      }

      template <typename... Types>
      types::none_type difference_update(types::empty_set const &set,
                                         Types const &... others)
      {
        // nothing can be removed in set
        return __builtin__::None;
      }

      PROXY_IMPL(pythonic::__builtin__::set, difference_update);
    }
  }
}
#endif
