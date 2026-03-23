#include "Files.h"
#include "alghoritms.h"
#include "lib/containers/list.h"
#include "logging.h"
#include "src/reader.h"
#include <format>
#include <fstream>
#include <memory>
#include <ostream>

std::function<long long(const std::unique_ptr<long long> &)> longToLong =
    [](const std::unique_ptr<long long> &datapoint) -> long long {
  return *datapoint;
};

int main(int argc, char *argv[]) {
  if (argc < 2 && isTxt(argv[1])) {
    Logger::critical(
        "Provide a valid input file in a .txt format as an argument");
  }
  Logger::debug("Extracting input data from path: {}", argv[1]);

  // Load the file
  UniqueList<Dataset> data = loadData(argv[1]);
  Logger::debug("Data loaded first datapoint {}", *(data->at(0)->data->at(0)));
  Logger::debug("Data loaded query size {}", (data->at(0)->query->size()));

  auto outFile =
      promptOpenFile<std::ofstream>("Provide an output path: ", isTxt);

  for (auto &dataFrame : *(data)) {
    Logger::debug(
        "Working on a dataFrame with {} length and {} searched values",
        dataFrame->data->size(), dataFrame->query->size());
    for (auto &query : *(dataFrame->query)) {
      // Dereference query to print its underlying value
      Logger::debug("Query is: {}", *query);

      int posInterpolated =
          interpolationSearch(*(dataFrame->data), query, longToLong);

      int posBinraySearched =
          binarySearch(*(dataFrame->data), query, longToLong);

      auto querySpan = findRangeBoundaries(*(dataFrame->data),
                                           posBinraySearched, longToLong);

      outFile << std::format(
          "({} {}) ",
          posInterpolated != -1 ? querySpan.second - querySpan.first + 1 : 0,
          posInterpolated);
    }
    outFile << std::endl;
    auto copiedContainer = uniqueElements(*dataFrame->data)->copy();
    auto &listRef =
        static_cast<List<std::unique_ptr<long long>> &>(*copiedContainer);
    listRef.slice(0, 200);
    for (int i = 0; i < listRef.size(); ++i) {
      outFile << std::format("{}{}", *listRef[i],
                             (i + 1) % 50 == 0 ? "\n" : " ");
    }
    outFile << std::endl;
  }
  outFile.close();
}
