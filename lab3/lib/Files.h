#pragma once

#include <functional>
#include <string>

template <typename StreamType>
StreamType promptOpenFile(std::string fileDesc,
                          std::function<bool(std::string)> verifier,
                          const std::string skipPath = "");
template <typename StreamType>
StreamType readFile(const std::string_view &fileName);
bool isTxt(std::string fileName);

#include "Files.inl"
