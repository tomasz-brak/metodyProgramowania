#include "array.h"
#include <cstddef>
#include <stdexcept>

template <typename T, int s>
void Array<T, s>::load(std::string &line,
                       std::function<T *(const std::string &)> converter) {
  throw std::runtime_error("Not implemented");
}
