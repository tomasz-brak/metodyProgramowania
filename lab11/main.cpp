#include <algorithm>
#include <chrono>
#include <cstddef>
#include <deque>
#include <functional>
#include <map>
#include <print>
#include <random>
#include <string>
#include <utility>
#include <vector>
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

void cTablica::sortbobel(testResult &test) {
  if (data.size() < 2) {
    return;
  }
  bool isSorted = false;
  while (!isSorted) {
    isSorted = true;
    for (auto it = data.begin(); it < data.end() - 1; it++) {
      test.comparisions++;
      if (*it > *(it + 1)) {
        isSorted = false;
        std::swap(*it, *(it + 1));
        test.swaps++;
      }
    }
  }
}

int partitionLomuto(size_t low, size_t high, cTablica &tab, testResult &test) {
  int pivot = tab.data[high];
  int i = low - 1;

  for (size_t j = low; j < high; ++j) {
    test.comparisions++;
    if (tab.data[j] <= pivot) {
      i++;
      std::swap(tab.data[j], tab.data[i]);
      test.swaps++;
    }
  }
  std::swap(tab.data[i + 1], tab.data[high]);
  test.swaps++;
  return i + 1;
}

void cTablica::sortlomuto(testResult &test) {
  if (this->data.size() < 2) {
    return;
  }
  int low = 0;
  int high = this->data.size() - 1;

  std::deque<std::pair<int, int>> q;
  q.push_back(std::make_pair(low, high));
  while (q.size() > 0) {
    auto [low, high] = q.back();
    q.pop_back();
    auto index = partitionLomuto(low, high, *this, test);
    test.comparisions++;
    if (index - 1 > low) {
      q.push_back(std::make_pair(low, index - 1));
    }
    test.comparisions++;
    if (index + 1 < high) {
      q.push_back(std::make_pair(index + 1, high));
    }
  }
}

void cTablica::sorthoare(testResult &test) {
  std::vector<std::pair<int, int>> stos;

  stos.push_back({0, data.size() - 1});

  while (!stos.empty()) {
    int low = stos.back().first;
    int high = stos.back().second;
    stos.pop_back();
    test.comparisions++;
    if (low < high) {
      int pivot = data[low + (high - low) / 2]; // elemenmt srodka przedzialu
      int i = low - 1;
      int j = high + 1;

      while (true) {
        do {
          // szuka po lewej stronie elementu wiekszego lub rownego pivot
          i++;
          test.comparisions++;
        } while (data[i] < pivot);

        do {
          // szuka po prawej stronie elementu mniejszego lub rownego pivot
          j--;
          test.comparisions++;
        } while (data[j] > pivot);
        // jesli i oraz j sie minely, przerywa petle
        test.comparisions++;
        if (i >= j) {
          break;
        }
        // jak sie nie minely to zamieniamy i oraz j miejscami
        std::swap(data[i], data[j]);
        test.swaps++;
      }
      int wynik = j;

      // odswiezanie stosu
      stos.push_back({low, wynik});
      stos.push_back({wynik + 1, high});
    }
  }
}

struct children {
  int *left;
  int *right;
};

void cTablica::sortkopcowanie(testResult &test) {
  if (this->data.size() < 2) {
    return;
  }

  int bound = 1;

  auto parent = [](int i) {
    if (i == 0)
      return -1;
    return (i - 1) / 2;
  };

  while (bound < data.size()) {
    int currentIdx = bound;
    bound++;

    while (currentIdx > 0) {
      int parentIdx = parent(currentIdx);

      test.comparisions++;
      if (this->data[currentIdx] > this->data[parentIdx]) {
        std::swap(this->data[currentIdx], this->data[parentIdx]);
        test.swaps++;

        currentIdx = parentIdx;
      } else {
        break;
      }
    }
  }
  int heapSize = this->data.size();

  while (heapSize > 1) {
    std::swap(this->data[0], this->data[heapSize - 1]);
    test.swaps++;

    heapSize--;

    int currentIdx = 0;
    while (true) {
      int leftChild = 2 * currentIdx + 1;
      int rightChild = 2 * currentIdx + 2;
      int largestIdx = currentIdx;

      test.comparisions++;
      if (leftChild < heapSize) {
        if (this->data[leftChild] > this->data[largestIdx]) {
          largestIdx = leftChild;
        }
      }

      test.comparisions++;
      if (rightChild < heapSize) {
        if (this->data[rightChild] > this->data[largestIdx]) {
          largestIdx = rightChild;
        }
      }

      test.comparisions++;
      if (largestIdx != currentIdx) {
        std::swap(this->data[currentIdx], this->data[largestIdx]);
        test.swaps++;
        currentIdx = largestIdx;
      } else {
        break;
      }
    }
  }
}

void cTablica::sortwach(testResult &test) {
  if (data.size() < 2)
    return;

  bool swapped = true;
  int start = 0;
  int end = data.size() - 1;

  while (swapped) {
    swapped = false;

    // przejscie w prawo
    for (int i = start; i < end; i++) {
      test.comparisions++;
      if (data[i] > data[i + 1]) {
        std::swap(data[i], data[i + 1]);
        test.swaps++;
        swapped = true;
      }
    }
    test.comparisions++;
    if (!swapped) {
      break;
    } // jesli tak to juz jest posortowana

    swapped = false;
    end--; // ostatnni element juz jest na swoim miejscu
    // przejscie w lewo
    for (int i = end - 1; i >= start; i--) {
      test.comparisions++;
      if (data[i] > data[i + 1]) {
        std::swap(data[i], data[i + 1]);
        test.swaps++;
        swapped = true;
      }
    }
    start++;
  }
}
void cTablica::test(std::function<void(cTablica *, testResult &)> sorter,
                    const std::string &name) {
  auto rng = std::default_random_engine{};
  std::ranges::shuffle(this->data, rng);
  std::println("Sorting with {}", name);
  // std::println("{} from:\n {}", name, this->data);
  auto test = testResult{};
  auto start = std::chrono::high_resolution_clock::now();
  sorter(this, test);
  auto end = std::chrono::high_resolution_clock::now();
  // std::println("to: {}", this->data);
  std::chrono::duration<double, std::milli> duration = end - start;
  std::println("comparisions done: {:05}, swaps done: {:05}, finished in: {}ms",
               test.comparisions, test.swaps, duration.count());
  std::println("------------------------------------------");
}
int main(int argc, char *argv[]) {
  cTablica tab{};
  tab.data = {1, 200, 302, 201, 3, 20, 18, 156, 73, 999, 1001, 0, 0, 0, 1};
  const std::map<std::string, std::function<void(cTablica *, testResult &)>>
      sorters = {{"Bąbelkowe", &cTablica::sortbobel},
                 {"Wachadłowe", &cTablica::sortwach},
                 {"Lomuto", &cTablica::sortlomuto},
                 {"Hoare", &cTablica::sorthoare},
                 {"przez kopcowanie", &cTablica::sortkopcowanie}};
  for (auto &[name, sorter] : sorters) {
    tab.test(sorter, name);
  }
  return 0;
}
