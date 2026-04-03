#include <exception>
#include <format>
#include <iostream>
#include <numeric>
#include <string>
#include <vector>

class ParserError : public std::exception {
private:
  std::string message;
  int tokenIndex;
  std::vector<std::string> context;
  mutable std::string formattedMessage;

public:
  template <typename T>
  ParserError(std::string msg, int index, const T &tokens)
      : message(msg), tokenIndex(index) {
    for (const auto &t : tokens) {
      if constexpr (std::is_same_v<std::decay_t<decltype(t)>, char>) {
        context.push_back(std::string(1, t));
      } else {
        context.push_back(std::string(t));
      }
    }
  }

  const char *what() const noexcept override {
    if (formattedMessage.empty()) {
      formattedMessage = std::format("Parser error occurred: {}", message);
    }
    return formattedMessage.c_str();
  }

  std::string visualize() const {
    std::string line = "";
    size_t arrowPos = 0;

    for (int i = 0; i < context.size(); ++i) {
      if (i == tokenIndex)
        arrowPos = line.length();
      line += context[i];
    }

    if (tokenIndex >= context.size())
      arrowPos = line.length();

    std::string indicator = "";
    if (arrowPos > 0)
      indicator = std::string(arrowPos - 1, '-') + "^";
    else
      indicator = "^";

    return std::format("{}\n    {}\n----{}", message, line, indicator);
  }
};
