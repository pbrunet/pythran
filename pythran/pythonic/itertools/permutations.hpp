#ifndef PYTHONIC_ITERTOOLS_PERMUTATIONS_HPP
#define PYTHONIC_ITERTOOLS_PERMUTATIONS_HPP

#include "pythonic/include/itertools/permutations.hpp"

#include "pythonic/__builtin__/range.hpp"
#include "pythonic/types/list.hpp"
#include "pythonic/utils/iterator.hpp"
#include "pythonic/utils/proxy.hpp"

#include <algorithm>
#include <iterator>
#include <vector>

namespace pythonic
{

  namespace itertools
  {
    namespace details
    {

      template <class T>
      permutations_iterator<T>::permutations_iterator(
          std::vector<typename T::value_type> &iter, size_t num_elts, bool end)
          : pool(iter), curr_permut(__builtin__::range(pool.size())),
            _size(num_elts), end(end)
      {
        if (num_elts > iter.size()) {
          end = true;
        }
      }

      template <class T>
      types::list<typename T::value_type> permutations_iterator<T>::
      operator*() const
      {
        types::list<typename T::value_type> res(_size);
        for (size_t i = 0; i < _size; i++)
          // Ok because types::list is indeed a vector
          res[i] = pool[curr_permut[i]];
        return res;
      }

      template <class T>
      permutations_iterator<T> &permutations_iterator<T>::operator++()
      {
        if (_size != pool.size()) {
          // Slow path, the iterator is a "view" of a prefix smaller
          // than the the pool size
          // FIXME a better implementation would be to avoid
          // std::next_permutation, but only in the slow path
          types::list<int> prev_permut(curr_permut.begin(),
                                       curr_permut.begin() + _size);
          types::list<int> new_permut;
          while ((end = std::next_permutation(curr_permut.begin(),
                                              curr_permut.end()))) {
            // Check if the prefix of the new permutation is
            // different of the previous one
            types::list<int> new_permut(curr_permut.begin(),
                                        curr_permut.begin() + _size);
            if (!(prev_permut == new_permut))
              break;
          }
        } else
          end = std::next_permutation(curr_permut.begin(), curr_permut.end());
        return *this;
      }

      template <class T>
      bool permutations_iterator<T>::
      operator!=(permutations_iterator<T> const &other) const
      {
        return !(*this == other);
      }

      template <class T>
      bool permutations_iterator<T>::
      operator==(permutations_iterator<T> const &other) const
      {
        if (other.end != end)
          return false;
        return std::equal(curr_permut.begin(), curr_permut.end(),
                          other.curr_permut.begin());
      }

      template <class T>
      bool permutations_iterator<T>::
      operator<(permutations_iterator<T> const &other) const
      {
        if (end != other.end)
          return end > other.end;
        for (int i = 0; i < pool.size(); i++)
          if (other.curr_permut[i] < curr_permut[i])
            return false;
          else if (other.curr_permut[i] > curr_permut[i])
            return true;
        return false;
      }

      template <class T>
      permutations<T>::permutations(T iter, long elts)
          : utils::iterator_reminder<false,
                                     std::vector<typename T::value_type>>(
                std::vector<typename T::value_type>(iter.begin(), iter.end())),
            iterator(this->value, elts, true),
            end_iter(this->value, elts, false)
      {
      }

      template <class T>
      typename permutations<T>::iterator const &permutations<T>::begin() const
      {
        return *this;
      }

      template <class T>
      typename permutations<T>::iterator permutations<T>::begin()
      {
        return *this;
      }

      template <class T>
      typename permutations<T>::iterator const &permutations<T>::end() const
      {
        return end_iter;
      }
    }

    template <typename T0>
    details::permutations<
        typename std::remove_cv<typename std::remove_reference<T0>::type>::type>
    permutations(T0 &&iter, long num_elts)
    {
      return {std::forward<T0>(iter), num_elts};
    }

    template <typename T0>
    details::permutations<
        typename std::remove_cv<typename std::remove_reference<T0>::type>::type>
    permutations(T0 &&iter)
    {
      return {std::forward<T0>(iter), std::distance(iter.begin(), iter.end())};
    }

    PROXY_IMPL(pythonic::itertools, permutations);
  }
}

#endif
