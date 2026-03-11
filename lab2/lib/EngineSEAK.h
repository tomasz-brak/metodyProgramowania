#pragma once

#include "Engine.h"
#include <istream>

class EngineSEAK : public Engine {
public:
  float Tz;
  float T1;
  float T2;

  void writeConsole() override;
  void load(std::istream &data) override;
  std::string getType() override;
};
