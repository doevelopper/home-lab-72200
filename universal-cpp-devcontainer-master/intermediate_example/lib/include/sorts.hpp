#pragma once
#include <iterator>
#include <stdexcept>
#include <type_traits>
#include <utility>

namespace example {

namespace detail {

template <typename ForwardIt>
void comb_sort(ForwardIt begin, ForwardIt end, float shrink_factor,
               std::forward_iterator_tag) {
  bool at_least_one_swap{true};
  while (at_least_one_swap) {
    at_least_one_swap = false;
    for (auto i{begin}; i != end; ++i) {
      if (*std::next(i) < *i) {
        std::swap(*i, *std::next(i));
        at_least_one_swap = true;
      }
    }
  }
}

template <typename BydirIt>
void comb_sort(BydirIt begin, BydirIt end, float shrink_factor,
               std::bidirectional_iterator_tag) {
  bool at_least_one_swap{true};
  while (begin != end-- && at_least_one_swap) {
    at_least_one_swap = false;
    for (auto i{begin}; i != end; ++i) {
      if (*std::next(i) < *i) {
        std::swap(*i, *std::next(i));
        at_least_one_swap = true;
      }
    }
  }
}

template <typename RandomIt>
void comb_sort(RandomIt begin, RandomIt end, float shrink_factor,
               std::random_access_iterator_tag) {
  typename std::iterator_traits<RandomIt>::difference_type gap =
      std::distance(begin, end) / shrink_factor;
  while (gap > 1) {
    for (auto i{begin}; i != end - gap; ++i) {
      if (*(i + gap) < *i)
        std::swap(*i, *(i + gap));
    }
    gap /= shrink_factor;
  }
  comb_sort(begin, end, shrink_factor,
            typename std::bidirectional_iterator_tag{});
}

} // namespace detail

template <typename ForwardIt>
void comb_sort(ForwardIt begin, ForwardIt end, float shrink_factor = 1.3f) {
  if (shrink_factor <= 1)
    throw std::invalid_argument("Shrink factor must be greater than 1");
  detail::comb_sort(
      begin, end, shrink_factor,
      typename std::iterator_traits<ForwardIt>::iterator_category{});
}

} // namespace example
