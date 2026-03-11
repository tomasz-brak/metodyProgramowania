#pragma once

#include "array.h"
#include <fstream>
#include <istream>
#include <string>
constexpr int DATA_SIZE = 100;
constexpr std::string SEPARATOR = " \t\n\r\f\v";

class Engine {

public:
  int model;
  int kp;
  float T;
  Array<float, DATA_SIZE> posY;
  Array<float, DATA_SIZE> posX;

  std::string rawData;

  virtual void writeConsole() = 0;
  virtual void load(std::istream &data) = 0;
  virtual std::string getType() = 0;

  std::string nextValue(std::string &data, std::string separator,
                        int occurence = 1);
  float mse(const Engine &other) const;

  template <typename ArrayType, int ArraySize>
    requires std::derived_from<ArrayType, Engine>
  static void findMatch(std::ofstream &output, Engine &unknown,
                        Array<ArrayType, ArraySize> engines);

protected:
  void loadDatapoints(std::string &line1, std::string &line2);
};

#include "Engine.inl"
