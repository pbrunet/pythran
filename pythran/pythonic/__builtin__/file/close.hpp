#ifndef PYTHONIC_BUILTIN_FILE_CLOSE_HPP
#define PYTHONIC_BUILTIN_FILE_CLOSE_HPP

#include "pythonic/include/__builtin__/file/close.hpp"

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

      types::none_type close(types::file &f)
      {
        f.close();
        return __builtin__::None;
      }

      types::none_type close(types::file &&f)
      {
        f.close();
        return __builtin__::None;
      }

      PROXY_IMPL(pythonic::__builtin__::file, close);
    }
  }
}
#endif
