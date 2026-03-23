#pragma once
#include "containers/list.h"
#include <string>

struct Dataset {
  UniqueList<long long> data =
      std::make_unique<List<std::unique_ptr<long long>>>();
  UniqueList<long long> query =
      std::make_unique<List<std::unique_ptr<long long>>>();
  std::unique_ptr<Dataset> copy() const;
};

UniqueList<Dataset> loadData(const std::string &path);
