#include "Files.h"
#include <regex>
#include <string>

/**
 * @brief Parse a file to get its file extention
 *
 * @param fileName filename
 * @return extention of the file
 */
std::string getFileExtention(const std::string &fileName) {
  static const std::regex re(R"(^(?:.*[\\/])?[^\\/\.][^\\/]*\.([^.\\/]+)$)");
  std::smatch m;
  if (std::regex_match(fileName, m, re) && m.size() >= 2) {
    return m[1].str();
  }
  return std::string{};
}

/**
 * @brief Check if filename has txt extention
 *
 * @param fileName fileName
 * @return true if is txt, false otherwise
 */
bool isTxt(std::string fileName) {
  auto ext = getFileExtention(fileName);
  return (ext == "txt");
}
