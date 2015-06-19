#ifndef PYTHONIC_INCLUDE_OPERATOR_ITEMGETTER_HPP
#define PYTHONIC_INCLUDE_OPERATOR_ITEMGETTER_HPP

#include "pythonic/include/types/tuple.hpp"
#include "pythonic/include/utils/proxy.hpp"
#include "pythonic/include/utils/seq.hpp"

namespace pythonic
{

  namespace operator_
  {
    namespace details
    {

      template <class T>
      struct itemgetter_return {
        T i;
        itemgetter_return() = default;
        itemgetter_return(T const &item);

        template <class A>
        auto operator()(A const &a) const -> decltype(a[i]);
      };

      template <class T, size_t N>
      struct itemgetter_tuple_return {

        types::array<T, N> items;

        template <class... Types>
        itemgetter_tuple_return(Types... &&items);

        itemgetter_tuple_return() = default;

        template <int... I>
        auto helper(utils::seq<I...>) const
            -> decltype(types::make_tuple(a[std::get<I>(items)]...));

        template <class A>
        auto operator()(A const &a) const
            -> decltype(helper(utils::gens<N>::type{}));
      };
    }

    template <class T>
    itemgetter_return<
        typename std::remove_cv<typename std::remove_reference<T>::type>::type>
    itemgetter(T &&item);

    template <class T1, class T2, class... L>
    details::itemgetter_tuple_return<
        typename std::remove_cv<typename std::remove_reference<T>::type>::type>,
        sizeof...(L) + 2 > itemgetter(T1 &&item1, T2 &&item2, L... items);

    PROXY_DECL(pythonic::operator_, itemgetter);
  }
}

#endif
