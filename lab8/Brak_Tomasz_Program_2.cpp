#include "lib/Files.h"
#include "src/backpack.h"
#include "src/error.h"
#include <fstream>
#include <print>

int main(int argc, char *argv[]) {
  std::ifstream file = promptOpenFile<std::ifstream>("", noCheck, "in2.txt");
  auto backpacks = Backpack::loadRecursive(file);
  for (auto &pack : backpacks) {
    try {
      pack.solveRecursive();
      std::println("{} = {}", pack.capacity, pack.backtrack());
    } catch (SolverFailedToConverge e) {
      std::println("BRAK");
    }
  }

  return 0;
}
