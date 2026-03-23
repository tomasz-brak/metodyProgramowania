#pragma once

#include "containers/container.h"
#include "logging.h"
#include <memory>
#include <string_view>

/**
 * @brief Extracts an arbirary amount of char from a string until it finds a
 * sparator. Line gets the chars removed.
 *
 * @param line Line to extract the chars from. Gets modified
 * @return Extracted token, without the separator.
 */
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

/**
 * @brief Loads a @ref #SEPARATORS
 *
 * @tparam T Type of the held item
 * @param input String to load from
 * @param converter Converter function to convert from string_view to #T
 */
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

/**
 * @brief Formatter specialization for @ref std::unique_ptr.
 *
 * If the pointer is valid, it delegates formatting to the formatter for type
 * #T. If the pointer is null, it outputs the string "null".
 *
 * @tparam T The type of the object managed by the unique_ptr.
 * @tparam CharT Character type
 */
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

/**
 * @brief Formatter specialization for @ref Container.
 *
 * Serializes the container with a space delimiter.
 *
 * @tparam T The type of elements stored in the @ref Container.
 * @tparam CharT Character type (typically char or wchar_t).
 */
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
