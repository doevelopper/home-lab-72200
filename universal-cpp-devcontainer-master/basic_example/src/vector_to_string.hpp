#ifndef EXAMPLE_PRINT_VECTOR
#define EXAMPLE_PRINT_VECTOR

#include <string>
#include <vector>

namespace example {
using std::to_string;

template <typename T> std::string to_string(std::vector<T> const &v) {
  std::string res = "std::vector{ ";
  for (auto const &i : v)
    res += to_string(i) + ", ";
  res.replace(res.size() - 2, 2, " }");
  return res;
}

template <> std::string to_string(std::vector<std::string> const &s);
} // namespace example

#endif
