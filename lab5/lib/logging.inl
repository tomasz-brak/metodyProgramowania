#pragma once
#include "logging.h"

/**
 * @brief Platform-agnostic helper for non-unicode printing
 */
void inline vprintCompatible(std::string_view fmt, std::format_args args) {
#if defined(_WIN32) || defined(_MSC_VER)
  // MSVC provides _buffered
  // https://github.com/alf-p-steinbach/C---how-to---make-non-English-text-work-in-Windows/blob/main/how-to-use-utf8-in-windows.md
  std::vprint_nonunicode_buffered(fmt, args);
#else
  // GCC/Clang (libstdc++/libc++)
  std::vprint_nonunicode(fmt, args);
#endif
}

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
  vprintCompatible(formatString, std::make_format_args(args...));
  std::print("\n");
}
