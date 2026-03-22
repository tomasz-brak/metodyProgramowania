#include "Files.h"
#include "lib/containers/list.h"
#include "logging.h"

int main(int argc, char *argv[]) {
  if (argc < 2 && isTxt(argv[1])) {
    Logger::critical("Provide a valid input file in a .txt format");
  }
  Logger::debug("Extracting input data from path: {}", argv[1]);

  // Load the file
}
