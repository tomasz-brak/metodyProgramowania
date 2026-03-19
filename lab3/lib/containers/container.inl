#pragma once

#include "containers/container.h"

std::string_view next_token(std::string_view &line) {
  const auto start = line.find_first_not_of(SEPARATORS);
  if (start == std::string_view::npos) {
    line = {};
    return {};
  }

  line.remove_prefix(start);

  const auto end = line.find_first_of(SEPARATORS);
  const auto token = line.substr(0, end);

  if (end == std::string_view::npos) {
    line = {};
  } else {
    line.remove_prefix(end);
  }

  return token;
}

template <typename T>
void Container<T>::load(
    std::string_view &line,
    std::function<T *(const std::string_view &)> converter) {
  while (!line.empty()) {
    std::string_view token = next_token(line);
    if (token.empty()) {
      break;
    }

    T *value = converter(token);
    if (value != nullptr) {
      append(*value);
    }
  }
}
