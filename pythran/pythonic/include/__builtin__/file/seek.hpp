#ifndef PYTHONIC_INCLUDE_BUILTIN_FILE_SEEK_HPP
#define PYTHONIC_INCLUDE_BUILTIN_FILE_SEEK_HPP

#include "pythonic/include/utils/proxy.hpp"
#include "pythonic/include/types/file.hpp"
#include "pythonic/include/types/none.hpp"

namespace pythonic
{

  namespace __builtin__
  {

    namespace file
    {

      types::none_type seek(types::file &f, long offset);
      types::none_type seek(types::file &&f, long offset);
      types::none_type seek(types::file &f, long offset, long whence);
      types::none_type seek(types::file &&f, long offset, long whence);

      PROXY_DECL(pythonic::__builtin__::file, seek);
    }
  }
}
#endif
