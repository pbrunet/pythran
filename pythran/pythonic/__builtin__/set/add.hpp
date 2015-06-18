#ifndef PYTHONIC_BUILTIN_SET_ADD_HPP
#define PYTHONIC_BUILTIN_SET_ADD_HPP

#include "pythonic/include/__builtin__/set/add.hpp"

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

      template <class T, class F>
      types::none_type add(types::set<T> &s, F const &value)
      {
        s.add(value);
        return __builtin__::None;
      }

      template <class T, class F>
      types::none_type add(types::set<T> &&s, F const &value)
      {
        // nothing have to be done as we work on rvalue
        return __builtin__::None;
      }

      PROXY_IMPL(pythonic::__builtin__::set, add);
    }
  }
}
#endif
