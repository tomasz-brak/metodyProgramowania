#include "lib/Files.h"
#include "src/backpack.h"
#include <fstream>
#include <memory>
#include <print>

int main(int argc, char *argv[]) {
  std::ifstream file = promptOpenFile<std::ifstream>("", noCheck, "in1.txt");
  auto backpack = std::make_unique<Backpack>(file);
  backpack->solveIterative();
  for (auto &line : backpack->packed) {
    std::println("{}", line);
  }

  for (auto &line : backpack->packed_idx) {
    std::println("{}", line);
  }
  return 0;
}
