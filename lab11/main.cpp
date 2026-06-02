#include <memory>
#include <print>
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

int main(int argc, char *argv[]) {
  std::println("Hello world");
  auto tab = cTablica();
  tab.sortbobel();
  std::println("Tablica {}", tab.data);
  return 0;
}
