#include "Files.h"
#include "containers/list.h"
#include "src/sets.h"
#include <format>
#include <fstream>
#include <iostream>
#include <print>

int main() {
  int n;
  int k;
  if (!(std::cin >> n >> k)) {
    return 1;
  }
  std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
  std::ofstream outFile = promptOpenFile<std::ofstream>(
      "Provide a path for the output file: ", isTxt);
  for (auto &subset : *subsetsLex(n, k)) {
    outFile << std::format("{}\n", subset);
  }
  return 0;
}
