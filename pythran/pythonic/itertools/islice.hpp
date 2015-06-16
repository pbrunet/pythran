#ifndef PYTHONIC_ITERTOOLS_ISLICE_HPP
#define PYTHONIC_ITERTOOLS_ISLICE_HPP

#include "pythonic/include/itertools/islice.hpp"

#include "pythonic/itertools/common.hpp"
#include "pythonic/utils/iterator.hpp"
#include "pythonic/utils/proxy.hpp"

#include <iterator>

namespace pythonic
{

  namespace itertools
  {
    namespace details
    {

      template <typename Iterable>
      islice_iterator<Iterable>::islice_iterator(Iterable &iterable, long begin,
                                                 long step)
          : _iterable(iterable.begin()), end_state(iterable.end()),
            current_state(0), step(step)
      {
        advance(
            std::is_same<typename std::iterator_traits<
                             typename Iterable::iterator>::iterator_category,
                         std::random_access_iterator_tag>(),
            begin);
      }

      template <typename Iterable>
      islice_iterator<Iterable>::islice_iterator(npos n, Iterable &iterable,
                                                 long end)
          : _iterable(iterable.end()), end_state(iterable.end()),
            current_state(end), step(1)
      {
      }

      template <typename Iterable>
      void
      islice_iterator<Iterable>::advance(std::true_type,
                                         long v) // random_access_iterator case
      {
        long dist_to_end = std::min(v, std::distance(_iterable, end_state));
        current_state += dist_to_end;
        _iterable += dist_to_end;
      }

      template <typename Iterable>
      void islice_iterator<Iterable>::advance(std::false_type,
                                              long v) // forward_iterator case
      {
        while (v) {
          ++_iterable;
          ++current_state;
          --v;
          if (_iterable == end_state)
            return;
        }
      }

      template <typename Iterable>
      typename Iterable::value_type islice_iterator<Iterable>::operator*() const
      {
        return *_iterable;
      }

      template <typename Iterable>
      islice_iterator<Iterable> &islice_iterator<Iterable>::operator++()
      {
        advance(
            std::is_same<typename std::iterator_traits<
                             typename Iterable::iterator>::iterator_category,
                         std::random_access_iterator_tag>(),
            step);
        return *this;
      }

      template <typename Iterable>
      bool islice_iterator<Iterable>::
      operator==(islice_iterator<Iterable> const &other) const
      {
        return (current_state == other.current_state ||
                _iterable == other._iterable);
      }

      template <typename Iterable>
      bool islice_iterator<Iterable>::
      operator!=(islice_iterator<Iterable> const &other) const
      {
        return !(*this == other);
      }

      template <typename Iterable>
      bool islice_iterator<Iterable>::
      operator<(islice_iterator<Iterable> const &other) const
      {
        return *this != other;
      }

      template <typename Iterable>
      long islice_iterator<Iterable>::
      operator-(islice_iterator<Iterable> const &other) const
      {
        return std::min(current_state - other.current_state,
                        std::distance(_iterable, end_state)) /
               step;
      }

      template <typename Iterable>
      template <typename Iter>
      islice<Iterable>::islice(Iter &&iterable, long begin, long end, long step)
          : utils::iterator_reminder<false, Iterable>(
                std::forward<Iter>(iterable)),
            iterator(this->value, begin, step),
            end_iter(npos(), this->value,
                     begin + long((end - begin + step - 1) / step) * step)
      {
      }

      template <typename Iterable>
      typename islice<Iterable>::iterator &islice<Iterable>::begin()
      {
        return *this;
      }

      template <typename Iterable>
      typename islice<Iterable>::iterator const &islice<Iterable>::begin() const
      {
        return *this;
      }

      template <typename Iterable>
      typename islice<Iterable>::iterator const &islice<Iterable>::end() const
      {
        return end_iter;
      }
    }

    template <typename Iterable>
    details::islice<typename std::remove_cv<
        typename std::remove_reference<Iterable>::type>::type>
    islice(Iterable &&iterable, long start, long stop, long step)
    {
      return {iterable, start, stop, step};
    }

    template <typename Iterable>
    details::islice<typename std::remove_cv<
        typename std::remove_reference<Iterable>::type>::type>
    islice(Iterable &&iterable, long stop)
    {
      return {iterable, 0, stop, 1};
    }

    PROXY_IMPL(pythonic::itertools, islice);
  }
}

#endif
