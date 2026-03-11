#include "EngineSEB.h"
#include <istream>
#include <print>
#include <stdexcept>
#include <string>

void EngineSEB::writeConsole() {
  std::print("SEB -- Model: {}, kp: {}, L: {}, R: {}, J: {}, T: {} -- "
             "Data: x{}; y{}; ...; x{}, y{}\n",
             model, kp, L, R, J, T, posX[0], posY[0], posX[DATA_SIZE - 1],
             posY[DATA_SIZE - 1]);
}

void EngineSEB::load(std::istream &data) {
  std::string line1, line2;
  if (!(std::getline(data, line1) && std::getline(data, line2))) {
    throw std::runtime_error("Line count mismatch!");
  }
  try {
    rawData = std::format("{}\n\n{}", line1, line2);
    model = std::stoi(nextValue(line1, SEPARATOR));
    kp = std::stoi(nextValue(line1, SEPARATOR));
    L = std::stoi(nextValue(line1, SEPARATOR));
    R = std::stof(nextValue(line1, SEPARATOR));
    J = std::stof(nextValue(line1, SEPARATOR));
    T = std::stof(nextValue(line1, SEPARATOR));

    loadDatapoints(line1, line2);
  } catch (std::invalid_argument &e) {
    std::print("Could not load data, malformed input: {}\n", e.what());
  }
}

std::string EngineSEB::getType() { return "SEB"; };
