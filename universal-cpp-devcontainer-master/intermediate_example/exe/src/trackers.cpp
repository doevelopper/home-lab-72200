#include "trackers.hpp"
#include <iostream>
#include <utility>

template <> void std::swap(example::TrackerInt &a, example::TrackerInt &b) {
  std::swap(a.value, b.value);
  std::cerr << "std::swap(" << a.value << ", " << b.value << ")\n";
}

template <>
void std::swap(example::TrackerDouble &a, example::TrackerDouble &b) {
  std::swap(a.value, b.value);
  std::cerr << "std::swap(" << a.value << ", " << b.value << ")\n";
}
