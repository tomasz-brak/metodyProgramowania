#include "./lib/logging.h"
#include "./src/Equasion.h"
#include "Files.h"
#include <algorithm>
#include <fstream>
#include <memory>
#include <string>
#include <unordered_set>
const std::unordered_set<char> VALID_CHARS = {
    'a', 'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j', 'k', 'l', 'm',
    'n', 'o', 'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x', 'y', 'z',
    '(', ')', '=', '<', '>', '+', '-', '*', '/', '%', '^', '~'};

int main() {
  std::ifstream inputFile = promptOpenFile<std::ifstream>(
      "File to read data from: ", isTxt, "dane.txt");
  std::ofstream outFile =
      promptOpenFile<std::ofstream>("File to write to: ", isTxt, "out.txt");
  Equation eq4;
  try {
    eq4.parseFromRPN("323+");
  } catch (const ParserError &e) {
    std::cout << e.visualize() << std::endl;
  }
  std::string line;
  std::getline(inputFile, line);
  int amount = std::stoi(line);
  for (int i = 0; i < amount; i++) {
    Equation eq;
    std::getline(inputFile, line);
    auto eqstr = std::string(line.substr(5));
    std::erase_if(eqstr, [](char c) { return !VALID_CHARS.contains(c); });
    if (line.substr(0, 3) == "INF") {
      outFile << "ONP: ";
      try {
        eq.parseFromINF(eqstr);
        outFile << eq.toRPN();
      } catch (const ParserError &e) {
        Logger::warn("Parser failed for eq{}, error:\n{}", i, e.visualize());
        outFile << "error";
      }
      outFile << "\n";
    } else if (line.substr(0, 3) == "ONP") {
      outFile << "INF: ";
      try {
        eq.parseFromRPN(eqstr);
        outFile << eq.toIFX();
      } catch (const ParserError &e) {
        Logger::warn("Parser failed for eq{}, error:\n{}", i, e.visualize());
        outFile << "error";
      }
      outFile << "\n";
    }
    Logger::debug("After parsing {} result is INF: {} RPN: {}", eqstr,
                  eq.toIFX(), eq.toRPN());
  }

  inputFile.close();
  outFile.close();

  return 0;
}
