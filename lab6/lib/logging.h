#pragma once

#include <array>
#include <print>
#include <string>
#include <string_view>

class Logger {
public:
  enum Level { Debug, Info, Warn, Error, Critical };

  struct LevelMap {
    Level lvl;
    std::string str;
  };

  static constexpr std::array<std::pair<Level, std::string_view>, 5>
      logLevelStrings{{{Debug, "Debug"},
                       {Info, "Info"},
                       {Warn, "Warn"},
                       {Error, "Error"},
                       {Critical, "Critical"}}};

  inline static constexpr std::array logColors = {
      "\033[36m", // Cyan
      "\033[32m", // Green
      "\033[33m", // Yellow
      "\033[31m", // Red
      "\033[35m"  // Magenta
  };

  inline static constexpr auto colorReset = "\033[0m";

  template <typename... Args>
  static inline void log(Level level, std::string formatString,
                         Args &&...args);

  template <typename... Args>
  static inline void debug(std::string fmt, Args &&...args) {
    log(Level::Debug, fmt, std::forward<Args>(args)...);
  }

  template <typename... Args>
  static inline void info(std::string fmt, Args &&...args) {
    log(Level::Info, fmt, std::forward<Args>(args)...);
  }

  template <typename... Args>
  static inline void warn(std::string fmt, Args &&...args) {
    log(Level::Warn, fmt, std::forward<Args>(args)...);
  }

  template <typename... Args>
  static inline void error(std::string fmt, Args &&...args) {
    log(Level::Error, fmt, std::forward<Args>(args)...);
  }

  template <typename... Args>
  static inline void critical(std::string fmt, Args &&...args) {
    log(Level::Critical, fmt, std::forward<Args>(args)...);
  }
};

#include "logging.inl"
