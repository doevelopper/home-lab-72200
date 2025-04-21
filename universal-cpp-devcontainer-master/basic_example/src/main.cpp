#include "pi.hpp"
#include "vector_to_string.hpp"
#include <iostream>
#include <string>
#include <vector>

int main() {
  std::vector<int> v1{1, 2, 3};
  std::vector<std::string> v2{"I", "love", "C++"};
  std::vector<std::vector<std::string>> v3{
      {"one", "two"}, {"apple", "orange"}, v2};

  std::cout << example::to_string(v1) << '\n';
  std::cout << example::to_string(v2) << '\n';
  std::cout << example::to_string(v3) << '\n';

  std::cout << example::pi << std::endl;

  return 0;
}