#pragma once

#include "Engine.h"
#include <concepts>
#include <limits>

/**
 * @brief Calculates the MSE for engines and and unknown one, writes data about
 * the one with the lowest mse as well as some additional information about
 * other matches.
 *
 * @tparam ArrayType Type of the engine
 * @param output output stream to write to
 * @param unknown the engine to check against
 * @param engines array of the reference engine data
 */
template <typename ArrayType, int ArraySize>
  requires std::derived_from<ArrayType, Engine>
void Engine::findMatch(std::ofstream &output, Engine &unknown,
                       Array<ArrayType, ArraySize> engines) {

  output << "Wartosci MSE dla silnika " << unknown.getType() << ":\n\n";

  float minimal = std::numeric_limits<float>::max();
  std::string minimalData;
  for (Engine &engine : engines) {
    auto mse = unknown.mse(engine);
    output << mse << " ";
    if (minimal > mse) {
      minimal = mse;
      minimalData = engine.rawData;
    }
  }

  output << "\n\nMinimalny blad MSE: " << minimal << std::endl;
  output << "\nWektor Y,X z wszystkimi danymi najlepszego dopasowania:\n";

  output << minimalData << std::endl;
}

