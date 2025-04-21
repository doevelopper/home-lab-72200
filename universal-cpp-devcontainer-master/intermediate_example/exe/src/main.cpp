#include "array_to_string.hpp"
#include "sorts.hpp"
#include "trackers.hpp"
#include <array>
#include <iostream>
#include <random>

int main() {
  std::random_device RNG;
  std::mt19937 pRNG(RNG());

  std::uniform_int_distribution<int> dist_int(-30, 30);
  std::array<example::TrackerInt, 10> arri{};
  for (auto &i : arri)
    i.value = dist_int(pRNG);
  std::cout << example::to_string(arri) << "\n";
  example::comb_sort(arri.begin(), arri.end());
  std::cout << example::to_string(arri) << "\n" << std::endl;

  std::uniform_real_distribution<double> dist_double(-1., 1.);
  std::array<example::TrackerDouble, 10> arrd{};
  for (auto &d : arrd)
    d.value = dist_double(pRNG);
  std::cout << example::to_string(arrd) << "\n";
  example::comb_sort(arrd.begin(), arrd.end());
  std::cout << example::to_string(arrd) << "\n" << std::endl;

  return 0;
}