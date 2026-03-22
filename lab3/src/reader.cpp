
#include "reader.h"
#include "containers/list.h"
#include "logging.h"
#include <fstream>
#include <memory>
#include <optional>
#include <sstream>
std::optional<std::istringstream> oneLine(std::ifstream &dataStream) {
  std::string line;
  std::getline(dataStream, line);
  std::istringstream iss(line);
  if (iss.fail()) {
    Logger::error("Failed to load a line into a string stream");
    return std::nullopt;
  }
  return iss;
}

std::unique_ptr<List<Dataset>> load_data(const std::string_view &path) {
  std::ifstream file = readFile<std::ifstream>(path);
  auto loadedData = std::make_unique<List<Dataset>>;
  int dataSetAmount = 1;
  int dataSize = 1;
  int querySize = 1;
  Dataset currentFrame;

  // Amount of data frames
  auto line = oneLine(file);
  if (line) {
    line.value() >> dataSetAmount;
  }

  // Amount of data
  line = oneLine(file);
  if (line) {
    line.value() >> dataSize;
  }

  // Data itself
  line = oneLine(file);
  if (line) {
    std::string sline = line.value().str();
    currentFrame.data->load(sline, std::stoi);
  }
}
