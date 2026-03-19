#pragma once
#include "logging.h"

template <typename... Args>
void Logger::log(Level level, std::string_view formatString, Args &&...args) {
  const auto idx = static_cast<std::size_t>(level);

  // Formatowanie: Kolor[POZIOM]ResetTreść
  std::print("{}[{}]{} ", logColors[idx], logLevelStrings[idx].second,
             colorReset);
  std::vprint_nonunicode(formatString, std::make_format_args(args...));
  std::print("\n");
}
