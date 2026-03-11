#pragma once

#include "Engine.h"
#include <concepts>
#include <limits>

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
