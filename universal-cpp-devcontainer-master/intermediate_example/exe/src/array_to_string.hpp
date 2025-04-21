#ifndef EXAMPLE_PRINT_VECTOR
#define EXAMPLE_PRINT_VECTOR

#include <array>
#include <cstddef>
#include <string>

namespace example {
using std::to_string;

template <typename T, std::size_t N>
std::string to_string(std::array<T, N> const &a) {
  std::string res = "std::array{ ";
  for (auto const &i : a)
    res += to_string(i) + ", ";
  res.replace(res.size() - 2, 2, " }");
  return res;
}

} // namespace example

#endif
