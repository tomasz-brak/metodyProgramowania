#include "EngineSEAK.h"
#include <format>
#include <istream>
#include <print>
#include <stdexcept>
#include <string>

void EngineSEAK::writeConsole() {
  std::print("SEAK -- Model: {}, kp: {}, Tz: {}, T1: {}, T2: {}, T: {} -- "
             "Data: x{}; y{}; ...; x{}, y{}\n",
             model, kp, Tz, T1, T2, T, posX[0], posY[0], posX[DATA_SIZE - 1],
             posY[DATA_SIZE - 1]);
}

void EngineSEAK::load(std::istream &data) {
  std::string line1, line2;
  if (!(std::getline(data, line1) && std::getline(data, line2))) {
    throw std::runtime_error("Line count mismatch!");
  }
  try {
    rawData = std::format("{}\n\n{}", line1, line2);
    model = std::stoi(nextValue(line1, SEPARATOR));
    kp = std::stoi(nextValue(line1, SEPARATOR));
    Tz = std::stoi(nextValue(line1, SEPARATOR));
    T1 = std::stof(nextValue(line1, SEPARATOR));
    T2 = std::stof(nextValue(line1, SEPARATOR));
    T = std::stof(nextValue(line1, SEPARATOR));

    loadDatapoints(line1, line2);
  } catch (std::invalid_argument &e) {
    std::print("Could not load data, malformed input: {}\n", e.what());
  }
}

std::string EngineSEAK::getType() { return "SEAK"; };
