#pragma once
#include <stdexcept>
class NoBackpackItemsProvided : std::logic_error {
  using std::logic_error::logic_error;
};

class SolverFailedToConverge : std::logic_error {
  using std::logic_error::logic_error;
};
