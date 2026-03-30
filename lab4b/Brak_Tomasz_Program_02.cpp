#include "./lib/logging.h"
#include "./src/Equasion.h"
#include <algorithm>
#include <memory>
#include <string>
int main() {
  Equation eq2;
  // eq2.parseToTree({"x", "=", "2", "*", "(", "1", "+", "2", ")", "-", "3",
  // "*",
  //                "(", "4", "+", "5", ")"});

  eq2.parseToTree({"3", "*", "(", "2", "+", "1", ")", "=", "9"});
  Logger::info("{}", eq2.toIFX());
  Logger::info("{}", eq2.toRPN());

  Equation eq3;
  auto s = std::string("12+3*56+4*-");
  eq3.parseFromRPN(s);
  Logger::info("{}", eq3);
  return 0;
}
