#include "src/error.h"
#include "src/solution.h"
#include <print>
#include <vector>
int main() {
  vec<datapoint<int>> data = {
      {'A', 12}, {'B', 7}, {'I', 18}, {'M', 10}, {'S', 9}, {'X', 5}, {'Z', 2},
  };

  auto res = solution(data);
  for (auto const &r : res) {
    std::print("{} -> {}\n", r.letter, r.codeword);
  }

  std::print("Dataset 2:\n");

  vec<datapoint<float>> d2 = {{'c', 0.11}, {'e', 0.22}, {'i', 0.16},
                              {'r', 0.12}, {'s', 0.15}, {'t', 0.10},
                              {'x', 0.14}};

  auto res2 = solution(d2);
  for (auto const &r : res2) {
    std::print("{} -> {}\n", r.letter, r.codeword);
  }
}
