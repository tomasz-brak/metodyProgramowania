#pragma once
#include <chrono>
#include <functional>

template <typename T> using vec = std::vector<T>;
struct testResult {
  std::chrono::duration<double> duration;
  int comparisions = 0;
  int swaps = 0;
};

class cTablica {
public:
  vec<int> data = {};
  void sortbobel(testResult &test);
  void sortwach(testResult &test);
  void sortlomuto(testResult &test);
  void sorthoare(testResult &test);
  void sortkopcowanie(testResult &test);

  void test(std::function<void(cTablica *, testResult &)> sorter,
            const std::string &name);
};
