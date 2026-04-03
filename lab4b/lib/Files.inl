#pragma once
#include "Files.h"
#include "logging.h"

#include <filesystem>
#include <functional>
#include <iostream>
#include <print>
#include <stdexcept>
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
    Logger::debug("Opening default path as it was not specified");
    filename = skipPath;
  }
  if (!verifier(filename)) {
    throw std::runtime_error(std::format(
        "Couldn't open a file due to an error in the file name. Filename {}",
        filename));
  }
  // We also search for the file in the top directory.
  if (std::filesystem::exists("./../../" + filename)) {
    Logger::debug("Opening a file in a top dir: {}", "./../../" + filename);
    return readFile<StreamType>("./../../" + filename);
  }
  // If a filename exists in the script dir we open that
  if (std::filesystem::exists(filename)) {
    Logger::debug("Opening a file...");
    return readFile<StreamType>(filename);
  }

  Logger::critical("Unable to open a file it does not exist...");
  throw std::runtime_error("");
};

/**
 * @brief Open a file
 *
 * @tparam StreamType Type of filestream
 * @param fileName name of the file to open
 * @return Opened file
 */
template <typename StreamType>
StreamType readFile(const std::string &fileName) {
  StreamType inFile((std::string(fileName)));
  if (inFile.is_open() && inFile.good()) {
    return inFile;
  }
  throw std::runtime_error(std::format("Could not open file {}", fileName));
}
