#include "reader.h"
#include "Files.h"
#include "containers/list.h"
#include "logging.h"
#include <fstream>
#include <memory>
#include <optional>
#include <sstream>
#include <string>
#include <string_view>
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

std::unique_ptr<Dataset> loadDataPoint(std::ifstream &file) {

  int dataSize = 1;
  int querySize = 1;
  auto currentFrame = std::make_unique<Dataset>();

  auto line = oneLine(file);
  // Amount of data
  if (line) {
    line.value() >> dataSize;
    Logger::debug("Sucessfuly loaded {} as the amount of data",
                  line.value().str());
  }

  currentFrame->data->ensure_capacity(dataSize);
  // Data itself
  line = oneLine(file);
  if (line) {
    std::string sline = line.value().str();
    currentFrame->data->load(
        sline,
        [](const std::string_view &s) mutable -> std::unique_ptr<long long> {
          return std::make_unique<long long>(std::stoll(std::string(s)));
        });
    Logger::debug("Sucessfuly loaded {} as the data", line.value().str());
  }

  // Amount of queries
  line = oneLine(file);
  if (line) {
    line.value() >> querySize;
    Logger::debug("Sucessfuly loaded {} as the amount of queries",
                  line.value().str());
  }

  currentFrame->query->ensure_capacity(querySize);
  // Queries themself
  line = oneLine(file);
  if (line) {
    std::string sline = line.value().str();
    currentFrame->query->load(
        sline,
        [](const std::string_view &s) mutable -> std::unique_ptr<long long> {
          return std::make_unique<long long>(std::stoll(std::string(s)));
        });
    Logger::debug("Sucessfuly loaded {} as the queries", line.value().str());
  }

  return currentFrame;
}

UniqueList<Dataset> loadData(const std::string_view &path) {
  Logger::debug("Loading data from path: {}", path);
  std::ifstream file = readFile<std::ifstream>(path);
  auto loadedData = std::make_unique<List<std::unique_ptr<Dataset>>>();
  int dataSetAmount = 1;

  auto line = oneLine(file);
  if (line) {
    line.value() >> dataSetAmount;

    Logger::debug("Sucessfuly loaded {} as the amount of frames",
                  line.value().str());
  }
  Logger::debug("Detected {} dataframes", dataSetAmount);
  loadedData->ensure_capacity(dataSetAmount);
  for (int i = 0; i < dataSetAmount; ++i) {
    Logger::debug("Loading dataframe {} out of {}", i + 1, dataSetAmount);
    loadedData->append(loadDataPoint(file));
  }
  return loadedData;
}

std::unique_ptr<Dataset> Dataset::copy() const {
  auto new_ds = std::make_unique<Dataset>();

  if (this->data) {
    auto cloned_data = this->data->copy();
    // Cast the Container back to the specific List
    new_ds->data = UniqueList<long long>(
        static_cast<List<std::unique_ptr<long long>> *>(cloned_data.release()));
  }

  if (this->query) {
    auto cloned_query = this->query->copy();
    new_ds->query =
        UniqueList<long long>(static_cast<List<std::unique_ptr<long long>> *>(
            cloned_query.release()));
  }

  return new_ds;
}
