#include <print>
#include <vector>

template <typename T> using vec = std::vector<T>;

class cTablica {
  vec<int> dane = {};
  void sortbobel();
  void sortwach();
  void sortlomuto();
  void sorthoare();
  void sortkopcowanie();
};

int main(int argc, char *argv[]) {
  std::println("Hello world");
  return 0;
}
