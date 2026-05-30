#pragma once

#include <stdexcept>
class NotImplementedError : public std::runtime_error {
public:
  explicit NotImplementedError()
      : std::runtime_error("You are not meant to call this method") {}
};

class NullComparisonError : public std::runtime_error {
public:
  explicit NullComparisonError()
      : std::runtime_error("One of the comparison elements is null") {}
};
