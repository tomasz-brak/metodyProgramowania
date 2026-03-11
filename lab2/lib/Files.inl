#pragma once
#include "Files.h"

#include <functional>
#include <iostream>
#include <print>
#include <string>
/**
 * @brief Promts the user for a filename to open
 *
 * @tparam StreamType What stream should open the file
 * @param fileDesc Description shown to the user
 * @param verifier Function to check the inputed file
 * @param skipPath what path to open if no file is provied
 * @return Stream with the file
 */
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

/**
 * @brief Open a file
 *
 * @tparam StreamType Type of filestream
 * @param fileName name of the file to open
 * @return Opened file
 */
template <typename StreamType> StreamType readFile(std::string fileName) {
  StreamType inFile(fileName);
  if (inFile.is_open()) {
    return inFile;
  }
  throw std::runtime_error(std::format("Could not open file {}", fileName));
}
