#include "containers/list.h"
#include "logging.h"
#include <stdexcept>

int main(int argc, char *argv[]) {
  if (argc < 2) {
    throw std::runtime_error("Not enough commmand line arguments, provide the "
                             "path to the input file");
  }
  Logger::debug("Extracting input data from path: {}", argv[1]);
  List<int> l1;
  l1.ensure_capacity(4);
  l1[0] = 1;
}
