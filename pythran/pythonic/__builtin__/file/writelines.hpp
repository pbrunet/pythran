#ifndef PYTHONIC_BUILTIN_FILE_WRITELINES_HPP
#define PYTHONIC_BUILTIN_FILE_WRITELINES_HPP

#include "pythonic/include/__builtin__/file/writelines.hpp"

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

      template <class F, class T>
      types::none_type writelines(F &&f, T const &sequence)
      {
        f.writelines(sequence);
        return __builtin__::None;
      }

      PROXY_IMPL(pythonic::__builtin__::file, writelines);
    }
  }
}
#endif
