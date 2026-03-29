#include "./lib/logging.h"
#include "./src/Equasion.h"
#include <memory>
int main() {
  Logger::info("Constructing a tree for a following equasion INF: 3x(1+2)");
  Equation eq;
  eq.RootOp = std::make_shared<Operation>();
  auto rop = eq.RootOp;
  ro Logger::info("{}", eq);
  return 0;
}
