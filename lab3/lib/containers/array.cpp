#include "array.h"
#include <cstddef>
#include <stdexcept>

template <typename T, int s>
void Array<T, s>::load(std::string_view &line,
                       std::function<T *(const std::string_view &)> converter) {
  throw std::runtime_error("Not implemented");
}
