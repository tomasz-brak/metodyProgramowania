#include "Engine.h"
#include <cmath>
#include <string>

std::string Engine::nextValue(std::string &data, std::string separator,
                              int occurence) {
  size_t terminator = 0;
  while (occurence > 0) {
    terminator = data.find_first_of(separator, terminator + 1);
    occurence--;
  }
  if (terminator == std::string::npos) {
    data = "";
    return data;
  }
  std::string value = data.substr(0, terminator);
  data = data.substr(terminator + 1);
  return value;
}

void Engine::loadDatapoints(std::string &line1, std::string &line2) {
  for (int i = 0; i < DATA_SIZE; i++) {
    posY[i] = std::stof(nextValue(line1, SEPARATOR));
  }

  // check if we don't skip any important data
  if (size_t len = line1.length(); len > 1) {
    throw std::runtime_error(
        std::format("Reading data failed! Skipped to the next line while {} "
                    "were still unread",
                    len));
  }

  // We skip 6 data points since they are irrelevant
  nextValue(line2, SEPARATOR, 6);

  for (int i = 0; i < DATA_SIZE; i++) {
    posX[i] = std::stof(nextValue(line2, SEPARATOR));
  }

  if (size_t len = line2.length(); len > 1) {
    throw std::runtime_error(
        std::format("Reading data failed! Skipped to the next line while {} "
                    "were still unread",
                    len));
  }
}

float Engine::mse(const Engine &other) const {
  float sum = 0;
  for (int i = 0; i < other.posX.size; ++i) {
    sum += std::pow(this->posX[i] - other.posX[i], 2);
    sum += std::pow(this->posY[i] - other.posY[i], 2);
  }
  return sqrt(sum);
}
