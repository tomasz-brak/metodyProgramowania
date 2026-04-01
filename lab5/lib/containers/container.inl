#pragma once

#include "containers/container.h"
#include "logging.h"
#include <format>
#include <memory>
#include <string>
#include <string_view>

/**
 * @brief Extracts an arbirary amount of char from a string until it finds a
 * sparator. Line gets the chars removed.
 *
 * @param line Line to extract the chars from. Gets modified
 * @return Extracted token, without the separator.
 */

inline std::string next_token(std::string &line) {
  const auto start = line.find_first_not_of(SEPARATORS);
  if (start == std::string::npos) {
    line.clear();
    return {};
  }

  line.erase(0, start);

  const auto end = line.find_first_of(SEPARATORS);
  const std::string token = line.substr(0, end);

  if (end == std::string::npos) {
    line.clear();
  } else {
    line.erase(0, end);
  }

  return token;
}

/**
 * @brief Loads a @ref #SEPARATORS
 *
 * @tparam T Type of the held item
 * @param input String to load from
 * @param converter Converter function to convert from std::string to #T
 */
template <typename T>
void Container<T>::load(
    const std::string &input,
    std::move_only_function<T(const std::string &) const> converter) {
  std::string line = input;
  while (!line.empty()) {
    std::string token = next_token(line);
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
struct std::formatter<std::unique_ptr<T>, CharT> {
  std::formatter<T, CharT> underlying;

  constexpr auto parse(std::format_parse_context &ctx) {
    return underlying.parse(ctx);
  }

  template <typename FormatContext>
  auto format(const std::unique_ptr<T> &ptr, FormatContext &ctx) const {
    if (ptr) {
      return std::format_to(ctx.out(), "{}", *ptr);
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
 * @tparam CharT Character type
 */
template <typename T, typename CharT>
struct std::formatter<Container<T>, CharT> : std::range_formatter<T, CharT> {
  constexpr formatter() {
    this->set_separator(" ");
    this->set_brackets("", "");
  }
};
