// Brak Tomasz
#include "src/search.h"
#include <iostream>
#include <print>
#include <span>
#include <stdexcept>
#include <string>
#include <vector>

template <typename SearchAlgo>
void runTest(const std::string &name, SearchAlgo searchAlgo, int target,
             std::vector<std::vector<int>> data) {
  auto isTargetPossibleInRow = [target](const std::vector<int> &row) {
    if (row.empty()) {
      throw std::invalid_argument("Row cannot be empty when searching.");
    }
    if (row.front() <= target && row.back() >= target)
      return 0;
    return (target < row.front()) ? 1 : -1;
  };

  auto isTarget = [target](int val) {
    if (val == target)
      return 0;
    return (target < val) ? 1 : -1;
  };

  int firstRow = searchAlgo(std::span<const std::vector<int>>(data),
                            isTargetPossibleInRow, false);
  if (firstRow != -1) {
    int firstCol =
        searchAlgo(std::span<const int>(data[firstRow]), isTarget, false);
    std::print("{}Pier: {} w ({}, {})\n", name, target, firstRow, firstCol);
  } else {
    std::print("{}Pier: nie ma {}\n", name, target);
  }

  int lastRow = searchAlgo(std::span<const std::vector<int>>(data),
                           isTargetPossibleInRow, true);
  if (lastRow != -1) {
    int lastCol =
        searchAlgo(std::span<const int>(data[lastRow]), isTarget, true);
    std::print("{}Ost: {} w ({}, {})\n", name, target, lastRow, lastCol);
  } else {
    std::print("{}Ost: nie ma {}\n", name, target);
  }
}

struct TestCase {
  int rows;
  int cols;
  int targetValue;
  std::vector<std::vector<int>> data;
};

int main() {
  auto runRecursive = [](auto data, auto enumFunc, bool find_last) {
    using T = typename decltype(data)::value_type;
    return bs_recursive<T>(data, enumFunc, find_last);
  };

  auto runIterative = [](auto data, auto enumFunc, bool find_last) {
    using T = typename decltype(data)::value_type;
    return bs_iterative<T>(data, enumFunc, find_last);
  };

  try {
    int datasetCount;
    if (!(std::cin >> datasetCount)) {
      return 0;
    }

    if (datasetCount < 0) {
      throw std::invalid_argument("Dataset count cannot be negative.");
    }

    std::vector<TestCase> testCases(datasetCount);

    for (int i = 0; i < datasetCount; i++) {
      int v;
      if (!(std::cin >> testCases[i].rows >> testCases[i].cols)) {
        throw std::runtime_error(
            "Failed to read rows and columns for test case.");
      }

      if (testCases[i].rows < 0 || testCases[i].cols < 0) {
        throw std::invalid_argument("Rows and columns cannot be negative.");
      }

      for (int j = 0; j < testCases[i].rows; j++) {
        testCases[i].data.emplace_back(std::vector<int>());
        testCases[i].data.back().reserve(testCases[i].cols);
        for (int k = 0; k < testCases[i].cols; k++) {
          if (!(std::cin >> v)) {
            throw std::runtime_error("Failed to read data element.");
          }
          testCases[i].data.back().push_back(v);
        }
      }

      if (!(std::cin >> testCases[i].targetValue)) {
        throw std::runtime_error("Failed to read target value.");
      }
    }

    std::print("\n");
    for (int i = 0; i < datasetCount; i++) {
      runTest("Rek", runRecursive, testCases[i].targetValue, testCases[i].data);
      runTest("Iter", runIterative, testCases[i].targetValue,
              testCases[i].data);
      std::print("---\n");
    }
  } catch (const std::exception &e) {
    std::cerr << "Error: " << e.what() << "\n";
    return 1;
  }

  return 0;
}
