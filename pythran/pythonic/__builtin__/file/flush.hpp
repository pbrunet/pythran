#ifndef PYTHONIC_BUILTIN_FILE_FLUSH_HPP
#define PYTHONIC_BUILTIN_FILE_FLUSH_HPP

#include "pythonic/include/__builtin__/file/flush.hpp"

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

      types::none_type flush(types::file &f)
      {
        f.flush();
        return __builtin__::None;
      }

      types::none_type flush(types::file &&f)
      {
        f.flush();
        return __builtin__::None;
      }

      PROXY_IMPL(pythonic::__builtin__::file, flush);
    }
  }
}
#endif
