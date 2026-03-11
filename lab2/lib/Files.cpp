#include "Files.h"
#include <regex>
#include <string>

std::string getFileExtention(const std::string &fileName) {
  static const std::regex re(R"(^(?:.*[\\/])?[^\\/\.][^\\/]*\.([^.\\/]+)$)");
  std::smatch m;
  if (std::regex_match(fileName, m, re) && m.size() >= 2) {
    return m[1].str();
  }
  return std::string{};
}
bool isTxt(std::string fileName) {
  auto ext = getFileExtention(fileName);
  return (ext == "txt");
}
