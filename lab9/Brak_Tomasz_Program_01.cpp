#include "src/error.h"
#include "src/solution.h"
#include <print>
int main(int argc, char *argv[]) {
  if (argc < 2) {
    throw NotEnoughArguments(
        "Not enough arguments were provied to the program, specify avaiable "
        "items file path as an argument.");
  }
  Solution s;
  s.load_file(argv[1]);
  std::println("Max weight is {}", s.backpack->capacity);
  for (Node &node : s.solve()) {
    std::println("Item {}, profit: {}, weight: {}", node.item_id, node.profit,
                 node.weight);
  }
  std::print("{}\n", *s.backpack);
}
