#include "src/cTablica.h"
#include <functional>
#include <map>
#include <string>
#include <vector>
template <typename T> using vec = std::vector<T>;

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
