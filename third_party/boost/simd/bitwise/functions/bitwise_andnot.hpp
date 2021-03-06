//==============================================================================
//         Copyright 2003 - 2012   LASMEA UMR 6602 CNRS/Univ. Clermont II
//         Copyright 2009 - 2012   LRI    UMR 8623 CNRS/Univ Paris Sud XI
//
//          Distributed under the Boost Software License, Version 1.0.
//                 See accompanying file LICENSE.txt or copy at
//                     http://www.boost.org/LICENSE_1_0.txt
//==============================================================================

#ifndef BOOST_SIMD_BITWISE_FUNCTIONS_BITWISE_ANDNOT_HPP_INCLUDED
#define BOOST_SIMD_BITWISE_FUNCTIONS_BITWISE_ANDNOT_HPP_INCLUDED
#include <boost/simd/include/functor.hpp>
#include <boost/dispatch/include/functor.hpp>


namespace boost { namespace simd { namespace tag
  {
    /*!
      @brief  bitwise_andnot generic tag

      Represents the bitwise_andnot function in generic contexts.

      @par Models:
      Hierarchy
    **/
    struct bitwise_andnot_ : ext::elementwise_<bitwise_andnot_>
    {
      /// @brief Parent hierarchy
      typedef ext::elementwise_<bitwise_andnot_> parent;
    };
  }
  /*!
    Computes the bitwise and not of its parameters.

    @par semantic:
    For any given value @c x, of type @c T1 @c y of type @c T2
    of same memory size:

    @code
    T1 r = bitwise_andnot(x, y);
    @endcode

    The code is equivalent to:

    @code
    T1 r = x & ~y;
    @endcode

    @par Alias

    b_andnot

    @see  @funcref{bitwise_and}, @funcref{bitwise_or}, @funcref{bitwise_xor}, @funcref{bitwise_notand},
    @funcref{bitwise_notor}, @funcref{bitwise_ornot}, @funcref{complement}
    @param  a0
    @param  a1

    @return      a value of the same type as the first input.

  **/
  BOOST_DISPATCH_FUNCTION_IMPLEMENTATION(tag::bitwise_andnot_, bitwise_andnot, 2)
  BOOST_DISPATCH_FUNCTION_IMPLEMENTATION(tag::bitwise_andnot_, b_andnot, 2)
} }

#endif
