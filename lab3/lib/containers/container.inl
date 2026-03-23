#pragma once

#include "containers/container.h"
#include "logging.h"
#include <memory>
#include <string_view>

inline std::string_view next_token(std::string_view &line) {
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
    const std::string_view &input,
    std::move_only_function<T(const std::string_view &) const> converter) {
  std::string_view line = input;
  while (!line.empty()) {
    std::string_view token = next_token(line);
    if (token.empty()) {
      break;
    }
    Logger::debug("Loading token: {}", token);
    T value = converter(token);
    append(std::move(value));
  }
}

template <typename T, typename CharT>
struct std::formatter<std::unique_ptr<T>, CharT> : std::formatter<T, CharT> {
  // Inheriting from std::formatter<T, CharT> provides parse,

  template <typename FormatContext>
  auto format(const std::unique_ptr<T> &ptr, FormatContext &ctx) const {
    if (ptr) {
      return std::formatter<T, CharT>::format(*ptr, ctx);
    }
    return std::format_to(ctx.out(), "null");
  }
};

template <typename T, typename CharT>
struct std::formatter<Container<T>, CharT> {
  constexpr auto parse(std::format_parse_context &ctx) { return ctx.begin(); }

  template <typename FormatContext>
  auto format(const Container<T> &container, FormatContext &ctx) const {
    auto out = ctx.out();
    bool first = true;
    for (const auto &elem : container) {
      if (!first) {
        out = std::format_to(out, " ");
      }
      out = std::format_to(out, "{}", elem);
      first = false;
    }
    return out;
  }
};
