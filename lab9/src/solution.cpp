#include "solution.h"
#include "backpack.h"
#include <fstream>
#include <memory>
#include <string>
void Solution::load_file(const std::string &filepath) {
  std::ifstream file(filepath);
  this->backpack = std::make_unique<Backpack>(file);
  file.close();
}
