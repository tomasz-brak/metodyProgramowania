#include "Engine.h"
#include <cmath>
#include <string>
#include <format>

/**
 * @brief Extracts a value from the front of a string up to a specified
 * separator
 *
 * @param data String containting the data
 * @param separator string containing all possible separators
 * @param occurence how many should be extracted (which instance of a separator
 * to search for) [default: 1]
 * @return the value as string (without the separator)
 */
std::string Engine::nextValue(std::string &data, std::string_view separator,
                              int occurence) {
  size_t terminator = std::string::npos;
  size_t startSearch = 0;

  for (int i = 0; i < occurence; ++i) {
    terminator = data.find_first_of(separator, startSearch);

    if (terminator == std::string::npos) {
      // If we are looking for the next value but no separator exists,
      // the value is the entire remaining string.
      std::string value = data;
      data = "";
      return value;
    }

    if (i < occurence - 1) {
      startSearch = terminator + 1;
    }
  }

  std::string value = data.substr(0, terminator);
  data = data.substr(terminator + 1);
  return value;
}

/**
 * @brief Extract datapoints
 *
 * @param line1 Y positions (already without the first 6 parameters)
 * @param line2 X positions (**with** the first 6)
 */
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

/**
 * @brief Calculate the *mse* for 2 engines
 *
 * @param other the other engine
 * @return the mse value
 */
float Engine::mse(const Engine &other) const {
  float sum = 0;
  for (int i = 0; i < other.posX.size; ++i) {
    sum += std::pow(this->posX[i] - other.posX[i], 2);
    sum += std::pow(this->posY[i] - other.posY[i], 2);
  }
  return sqrt(sum);
}
