#pragma once
#include "logging.h"

/**
 * @brief Shows a formatted message to the stdout
 *
 * @tparam Args Format arguments
 * @param level log level
 * @param formatString Format string
 * @param args arguments to be formatted into the message
 */
template <typename... Args>
void Logger::log(Level level, std::string formatString, Args &&...args) {
  const auto idx = static_cast<std::size_t>(level);

  std::print("{}[{}]{} ", logColors[idx], logLevelStrings[idx].second,
             colorReset);
  std::vprint_nonunicode_buffered(formatString, std::make_format_args(args...));
  std::print("\n");
}
