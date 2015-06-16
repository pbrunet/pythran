#ifndef PYTHONIC_INCLUDE_ITERTOOLS_ISLICE_HPP
#define PYTHONIC_INCLUDE_ITERTOOLS_ISLICE_HPP

#include "pythonic/include/itertools/common.hpp"
#include "pythonic/include/utils/iterator.hpp"
#include "pythonic/include/utils/proxy.hpp"

#include <iterator>

namespace pythonic
{

  namespace itertools
  {
    namespace details
    {
      template <typename Iterable>
      struct islice_iterator
          : std::iterator<typename std::iterator_traits<
                              typename Iterable::iterator>::iterator_category,
                          typename std::iterator_traits<
                              typename Iterable::iterator>::value_type> {

        typename Iterable::iterator _iterable;
        typename Iterable::iterator const end_state;

        long current_state;
        long step;

        islice_iterator() = default;
        islice_iterator(Iterable &iterable, long begin, long step = 1);
        islice_iterator(npos n, Iterable &iterable, long end);

        typename Iterable::value_type operator*() const;
        void advance(std::true_type, long v);
        void advance(std::false_type, long v);
        islice_iterator &operator++();
        bool operator==(islice_iterator const &other) const;
        bool operator!=(islice_iterator const &other) const;
        bool operator<(islice_iterator const &other) const;
        long operator-(islice_iterator const &other) const;
      };

      template <typename Iterable>
      struct islice : utils::iterator_reminder<false, Iterable>,
                      islice_iterator<Iterable> {

        using iterator = islice_iterator<Iterable>;
        using value_type = typename Iterable::value_type;

        iterator end_iter;

        islice() = default;
        template <class Iter> // Use an extra template for forwarding
        islice(Iter &&iterable, long begin, long end, long step);

        iterator &begin();
        iterator const &begin() const;
        iterator const &end() const;
      };
    }

    template <typename Iterable>
    details::islice<typename std::remove_cv<
        typename std::remove_reference<Iterable>::type>::type>
    islice(Iterable &&iterable, long start, long stop, long step = 1);

    template <typename Iterable>
    details::islice<typename std::remove_cv<
        typename std::remove_reference<Iterable>::type>::type>
    islice(Iterable &&iterable, long stop);

    PROXY_DECL(pythonic::itertools, islice);
  }
}

#endif
