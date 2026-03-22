#pragma once
#include "Files.h"
#include "containers/list.h"
#include <fstream>
#include <memory>
#include <string_view>

struct Dataset {
  std::unique_ptr<List<int>> data;
  std::unique_ptr<List<int>> query;
};

std::unique_ptr<List<Dataset>> load_data(const std::string_view &path);
