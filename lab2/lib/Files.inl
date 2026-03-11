#pragma once
#include "Files.h"

#include <functional>
#include <iostream>
#include <print>
#include <string>
template <typename StreamType>
StreamType promptOpenFile(std::string fileDesc,
                          std::function<bool(std::string)> verifier,
                          const std::string skipPath) {
  std::string filename;
  std::print("{}", fileDesc);
  std::getline(std::cin, filename);
  if (filename == "") {
    return readFile<StreamType>(skipPath);
  }
  if (!verifier(filename)) {
    throw std::runtime_error(std::format(
        "Couldn't open a file due to an error in the file name. Filename {}",
        filename));
  }
  return readFile<StreamType>(filename);
};

template <typename StreamType> StreamType readFile(std::string fileName) {
  StreamType inFile(fileName);
  if (inFile.is_open()) {
    return inFile;
  }
  throw std::runtime_error(std::format("Could not open file {}", fileName));
}
