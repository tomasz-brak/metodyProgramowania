#include <cmath>
#include <iostream>
#include <print>
#include <random>
#include <vector>
struct point {
  float x;
  float y;
};

// pi = 4 * (p_kola / p_kwadrat)
template <typename T> using vec = std::vector<T>;
int main(int argc, char *argv[]) {
  int punktyL;
  std::println("Podaj liczbe punktow");
  std::random_device rd;
  std::mt19937 mt(rd());
  std::cin >> punktyL;

  std::uniform_real_distribution<double> dis(-1.0, 1.0);
  int inside = 0;
  int outside = 0;
  for (int i = 0; i < punktyL; ++i) {
    float x = dis(rd);
    float y = dis(rd);
    if (pow(x, 2) + pow(y, 2) <= 1) {
      inside++;
    }
    outside++;
  }
  std::println("Inside: {}, Outside: {}, PI: {}", inside, outside,
               4.0 *
                   (static_cast<float>(inside) / static_cast<float>(outside)));
  return 0;
}
