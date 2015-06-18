#ifndef PYTHONIC_BUILTIN_FILE_SEEK_HPP
#define PYTHONIC_BUILTIN_FILE_SEEK_HPP

#include "pythonic/include/__builtin__/file/seek.hpp"

#include "pythonic/__builtin__/None.hpp"
#include "pythonic/types/file.hpp"
#include "pythonic/types/none.hpp"
#include "pythonic/utils/proxy.hpp"

namespace pythonic
{

  namespace __builtin__
  {

    namespace file
    {

      types::none_type seek(types::file &f, long offset)
      {
        f.seek(offset);
        return __builtin__::None;
      }

      types::none_type seek(types::file &&f, long offset)
      {
        // Nothing have to be done as it is a lvalue
        return __builtin__::None;
      }

      types::none_type seek(types::file &f, long offset, long whence)
      {
        f.seek(offset, whence);
        return __builtin__::None;
      }

      types::none_type seek(types::file &&f, long offset, long whence)
      {
        // Nothing have to be done as it is a lvalue
        return __builtin__::None;
      }

      PROXY_IMPL(pythonic::__builtin__::file, seek);
    }
  }
}
#endif
