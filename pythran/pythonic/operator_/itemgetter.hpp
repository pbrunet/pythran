#ifndef PYTHONIC_OPERATOR_ITEMGETTER_HPP
#define PYTHONIC_OPERATOR_ITEMGETTER_HPP

#include "pythonic/include/operator_/itemgetter.hpp"

#include "pythonic/types/tuple.hpp"
#include "pythonic/utils/seq.hpp"
#include "pythonic/utils/proxy.hpp"

namespace pythonic
{

  namespace operator_
  {
    namespace details
    {

      template <class T>
      itemgetter_return::itemgetter_return<T>(T const &item)
          : i(item)
      {
      }

      template <class T>
      template <class A>
      auto itemgetter_return<T>::operator()(A const &a) const -> decltype(a[i])
      {
        return a[i];
      }

      template <class T, size_t N>
      template <class... Types>
      itemgetter_tuple_return<T, N>::itemgetter_tuple_return(Types... &&items)
          : items(std::forward<Types>(items)...)
      {
      }

      template <typename T, size_t N>
      template <int... I>
      auto itemgetter_tuple_return<T, N>::helper(utils::seq<I...>) const
          -> decltype(types::make_tuple(a[std::get<I>(items)]...))
      {
        return types::make_tuple(a[std::get<I>(items)]...);
      }

      template <typename T, size_t N>
      template <class A>
      auto itemgetter_tuple_return<T, N>::operator()(A const &a) const
          -> decltype(helper(utils::gens<N>::type{}))
      {
        return helper(utils::gens<N>::type{});
      }
    }

    template <class T>
    itemgetter_return<
        typename std::remove_cv<typename std::remove_reference<T>::type>::type>
    itemgetter(T &&item)
    {
      return {std::forward<T>(item)};
    }

    template <class T1, class T2, class... L>
    details::itemgetter_tuple_return<
        typename std::remove_cv<typename std::remove_reference<T>::type>::type>,
        sizeof...(L) + 2 > itemgetter(T1 &&item1, T2 &&item2, L... items)
    {
      return {std::forward<T1>(item1), std::forward<T2>(item2),
              std::forward<L>(items)...};
    }

    PROXY_IMPL(pythonic::operator_, itemgetter);
  }
}

#endif
