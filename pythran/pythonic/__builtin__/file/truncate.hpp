#ifndef PYTHONIC_BUILTIN_FILE_TRUNCATE_HPP
#define PYTHONIC_BUILTIN_FILE_TRUNCATE_HPP

#include "pythonic/include/__builtin__/file/truncate.hpp"

#include "pythonic/types/file.hpp"
#include "pythonic/types/none.hpp"
#include "pythonic/__builtin__/None.hpp"
#include "pythonic/utils/proxy.hpp"

namespace pythonic
{

  namespace __builtin__
  {

    namespace file
    {

      types::none_type truncate(types::file &f)
      {
        f.truncate();
        return __builtin__::None;
      }

      types::none_type truncate(types::file &&f)
      {
        f.truncate();
        return __builtin__::None;
      }

      types::none_type truncate(types::file &f, long size)
      {
        f.truncate(size);
        return __builtin__::None;
      }

      types::none_type truncate(types::file &&f, long size)
      {
        f.truncate(size);
        return __builtin__::None;
      }

      PROXY_IMPL(pythonic::__builtin__::file, truncate);
    }
  }
}
#endif
