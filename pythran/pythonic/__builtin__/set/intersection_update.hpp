#ifndef PYTHONIC_BUILTIN_SET_INTERSECTIONUPDATE_HPP
#define PYTHONIC_BUILTIN_SET_INTERSECTIONUPDATE_HPP

#include "pythonic/include/__builtin__/set/intersection_update.hpp"

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

      template <typename T, typename... Types>
      types::none_type intersection_update(types::set<T> &set,
                                           Types const &... others)
      {
        set.intersection_update(others...);
        return __builtin__::None;
      }

      template <typename T, typename... Types>
      types::none_type intersection_update(types::set<T> &&set,
                                           Types const &... others)
      {
        // If it is an rvalue, we don't really want to update
        return __builtin__::None;
      }

      template <typename... Types>
      types::none_type intersection_update(types::empty_set &&set,
                                           Types const &... others)
      {
        // If it is an empty_set, it is not really updated otherwise we have a
        // typing issue
        return __builtin__::None;
      }

      PROXY_IMPL(pythonic::__builtin__::set, intersection_update);
    }
  }
}
#endif
