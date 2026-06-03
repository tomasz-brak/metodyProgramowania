#include <cstddef>
#include <deque>
#include <memory>
#include <print>
#include <ranges>
#include <utility>
#include <vector>

template <typename T> using vec = std::vector<T>;

class cTablica {
public:
  vec<int> data = {32, 2, 13, 14, 17, 93, 1003, 3, 1004};
  void sortbobel();
  void sortwach();
  void sortlomuto();
  void sorthoare();
  void sortkopcowanie();
};

void cTablica::sortbobel() {
  if (data.size() < 2) {
    return;
  }
  bool isSorted = false;
  while (!isSorted) {
    isSorted = true;
    for (auto it = data.begin(); it < data.end() - 1; it++) {
      if (*it > *(it + 1)) {
        isSorted = false;
        std::swap(*it, *(it + 1));
      }
    }
  }
}

int partitionLomuto(size_t low, size_t high, cTablica &tab) {
  int pivot = tab.data[high];
  int i = low - 1;

  for (size_t j = low; j < high; ++j) {
    if (tab.data[j] <= pivot) {
      i++;
      std::swap(tab.data[j], tab.data[i]);
    }
  }
  std::swap(tab.data[i + 1], tab.data[high]);
  return i + 1;
}

void cTablica::sortlomuto() {
  int low = 0;
  int high = this->data.size() - 1;

  std::deque<std::pair<int, int>> q;
  q.push_back(std::make_pair(low, high));
  while (q.size() > 0) {
    auto [low, high] = q.back();
    q.pop_back();
    auto index = partitionLomuto(low, high, *this);
    if (index - 1 > low) {
      q.push_back(std::make_pair(low, index - 1));
    }
    if (index + 1 < high) {
      q.push_back(std::make_pair(index + 1, high));
    }
  }
}

int main(int argc, char *argv[]) {
  std::println("Hello world");
  auto tab = cTablica();
  tab.sortlomuto();
  std::println("Tablica {}", tab.data);
  return 0;
}
