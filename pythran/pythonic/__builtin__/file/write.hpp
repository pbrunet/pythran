#ifndef PYTHONIC_BUILTIN_FILE_WRITE_HPP
#define PYTHONIC_BUILTIN_FILE_WRITE_HPP

#include "pythonic/include/__builtin__/file/write.hpp"

#include "pythonic/__builtin__/None.hpp"
#include "pythonic/types/file.hpp"
#include "pythonic/types/none.hpp"
#include "pythonic/types/str.hpp"
#include "pythonic/utils/proxy.hpp"

namespace pythonic
{

  namespace __builtin__
  {

    namespace file
    {

      types::none_type write(types::file &f, types::str const &str)
      {
        f.write(str);
        return __builtin__::None;
      }

      types::none_type write(types::file &&f, types::str const &str)
      {
        f.write(str);
        return __builtin__::None;
      }

      PROXY_IMPL(pythonic::__builtin__::file, write);
    }
  }
}
#endif
