#ifndef PYTHONIC_INCLUDE_ITERTOOLS_PERMUTATIONS_HPP
#define PYTHONIC_INCLUDE_ITERTOOLS_PERMUTATIONS_HPP

#include "pythonic/include/types/list.hpp"
#include "pythonic/include/utils/iterator.hpp"
#include "pythonic/include/utils/proxy.hpp"

#include <iterator>
#include <vector>

namespace pythonic
{

  namespace itertools
  {
    namespace details
    {
      /** Permutation iterator
       *
       *  It wraps a vector and provide an iteration over every possible
       *  permutation of the vector. The permutations are represented as lists
       *  of elements.
       *
       *  The following iterator:
       *
       *  permutations_iterator([0, 1, 2])
       *
       *  yelds the following suite:
       *
       *  [(0, 1, 2), (0, 2, 1), (1, 0, 2), (1, 2, 0), (2, 0, 1), (2, 1, 0)]
       */
      template <class T>
      struct permutations_iterator
          : std::iterator<std::forward_iterator_tag,
                          types::list<typename T::value_type>, ptrdiff_t,
                          types::list<typename T::value_type> *,
                          types::list<typename T::value_type> /* no ref*/
                          > {
        // Vector of inputs, contains elements to permute
        std::vector<typename T::value_type> &pool;

        // The current permutation as a list of index in the pool
        // Internally it always has the same size as the pool, even if the
        // external view is limited
        types::list<int> curr_permut;

        // Size of the "visible" permutation
        size_t _size;
        bool end; // sentinel marker

        permutations_iterator() = default;
        permutations_iterator(std::vector<typename T::value_type> &iter,
                              size_t num_elts, bool end);

        /** Build the permutation visible from the "outside" */
        types::list<typename T::value_type> operator*() const;

        /*  Generate next permutation
         *
         *  If the size of the permutation is smaller than the size of the
         *  pool, we may have to iterate multiple times
         */
        permutations_iterator &operator++();
        bool operator!=(permutations_iterator const &other) const;
        bool operator==(permutations_iterator const &other) const;
        bool operator<(permutations_iterator const &other) const;
      };

      template <class T>
      struct permutations
          : utils::iterator_reminder<false,
                                     std::vector<typename T::value_type>>,
            permutations_iterator<T> {
        using value_type = types::list<typename T::value_type>;
        using iterator = permutations_iterator<T>;

        iterator const end_iter;

        permutations() = default;
        permutations(T iter, long elts);

        iterator const &begin() const;
        iterator begin();
        iterator const &end() const;
      };
    }

    template <typename T0>
    details::permutations<
        typename std::remove_cv<typename std::remove_reference<T0>::type>::type>
    permutations(T0 &&iter, long num_elts);

    template <typename T0>
    details::permutations<
        typename std::remove_cv<typename std::remove_reference<T0>::type>::type>
    permutations(T0 &&iter);

    PROXY_DECL(pythonic::itertools, permutations);
  }
}

#endif
