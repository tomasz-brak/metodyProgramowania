#include "array.h"
#include <cstddef>

template <typename T, int s>
void Array<T, s>::load(std::string_view &line,
                       std::function<T *(const std::string_view &)> converter) {
  auto nextBite = line.substr(0, line.find(SEPARATORS));
  size_t idx = 0;
  while (line != "") {

    idx++;
  }
}
