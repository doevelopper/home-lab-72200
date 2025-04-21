#pragma once

#include <string>
#include <utility>

namespace example {
using std::to_string;

struct TrackerInt {
  int value;

  bool operator<(TrackerInt const &other) { return this->value < other.value; }
};

struct TrackerDouble {
  double value;

  bool operator<(TrackerDouble const &other) {
    return this->value < other.value;
  }
};

inline std::string to_string(TrackerInt const &i) noexcept {
  return to_string(i.value);
}
inline std::string to_string(TrackerDouble const &d) noexcept {
  return to_string(d.value);
}

} // namespace example

template <> void std::swap(example::TrackerInt &a, example::TrackerInt &b);

template <>
void std::swap(example::TrackerDouble &a, example::TrackerDouble &b);
