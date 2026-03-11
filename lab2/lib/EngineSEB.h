#pragma once

#include "Engine.h"
#include <istream>

class EngineSEB : public Engine {
public:
  float L;
  float R;
  float J;

  void writeConsole() override;
  void load(std::istream &data) override;

  std::string getType() override;
};
