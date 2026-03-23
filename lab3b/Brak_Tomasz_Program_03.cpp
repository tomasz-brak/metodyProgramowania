#include "Files.h"
#include "src/sets.h"
#include <format>
#include <fstream>
#include <iostream>

int main() {
  int n;
  if (!(std::cin >> n))
    return 1;
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  std::ofstream outFile = promptOpenFile<std::ofstream>(
      "Provide a path for the output file: ", isTxt);
  for (auto &subset : *generatePartitions(n)) {
    outFile << std::format("{}\n", subset);
  }
  return 0;
}
