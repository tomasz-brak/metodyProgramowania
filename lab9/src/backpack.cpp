#include "backpack.h"
#include "error.h"
#include <algorithm>
#include <fstream>
#include <istream>
#include <iterator>
#include <sstream>
#include <string>
#include <vector>

std::istream &operator>>(std::istream &is, Backpack::Item &item) {
  is >> item.name >> item.mass >> item.price;
  return is;
}

std::istream &operator>>(std::istream &is, Backpack::SimpleItem &item) {
  is >> item.mass;
  return is;
}

Backpack::Backpack(std::ifstream &file) {
  file >> this->capacity;
  std::for_each(std::istream_iterator<Backpack::Item>(file),
                std::istream_iterator<Backpack::Item>(),
                [this](const Backpack::Item &i) { this->items.push_back(i); });

  this->packed.push_back(std::vector<int>(capacity + 1, 0));
  this->packed_idx.push_back(std::vector<int>(capacity + 1, -1));
  for (auto &item : this->items) {
    this->packed.push_back(std::vector<int>(capacity + 1, 0));
    this->packed_idx.push_back(std::vector<int>(capacity + 1, -1));
  }
}

void Backpack::solveIterative() {
  for (int i = 1; i <= items.size(); i++) {
    for (int j = 0; j <= this->capacity; j++) {
      int current_item_mass = items[i - 1].mass;
      int current_item_price = items[i - 1].price;

      if (current_item_mass > j) {
        packed[i][j] = packed[i - 1][j];
        packed_idx[i][j] = packed_idx[i - 1][j];
      } else {
        int exclude_price = packed[i - 1][j];
        int include_price =
            packed[i - 1][j - current_item_mass] + current_item_price;

        if (include_price > exclude_price) {
          packed[i][j] = include_price;
          packed_idx[i][j] = i - 1;
        } else {
          packed[i][j] = exclude_price;
          packed_idx[i][j] = packed_idx[i - 1][j];
        }
      }
    }
  }
}

int Backpack::recursiveHelper(int i, int j) {
  if (i <= 0 || j <= 0) {
    return 0;
  }

  if (packed_idx[i][j] != -1) {
    return packed[i][j];
  }

  int current_item_mass = items[i - 1].mass;
  int current_item_price = items[i - 1].price;

  if (current_item_mass > j) {
    packed[i][j] = recursiveHelper(i - 1, j);
    packed_idx[i][j] = packed_idx[i - 1][j];
  } else {
    int exclude_price = recursiveHelper(i - 1, j);

    int include_price =
        recursiveHelper(i - 1, j - current_item_mass) + current_item_price;

    if (include_price > exclude_price) {
      packed[i][j] = include_price;
      packed_idx[i][j] = i - 1;
    } else {
      packed[i][j] = exclude_price;
      packed_idx[i][j] = packed_idx[i - 1][j];
    }
  }

  return packed[i][j];
}

void Backpack::solveRecursive() {
  if (recursiveHelper(items.size(), this->capacity) != this->capacity) {
    throw SolverFailedToConverge("Failed to solve and fill the backpack");
  }
}
std::vector<Backpack> Backpack::loadRecursive(std::ifstream &inFile) {
  std::string datasetCount;
  if (!std::getline(inFile, datasetCount) || datasetCount.empty())
    return {};
  void load_file(const std::string &filepath);

  std::vector<Backpack> backpacks;
  while (true) {
    std::string maxCapacity, maxElementCount, elementsLine;
    if (!std::getline(inFile, maxCapacity) || maxCapacity.empty())
      break;
    if (!std::getline(inFile, maxElementCount))
      break;
    if (!std::getline(inFile, elementsLine))
      break;

    int cap = std::stoi(maxCapacity);
    int count = std::stoi(maxElementCount);

    backpacks.push_back(Backpack(cap, count));
    auto &last = backpacks.back();

    std::stringstream elements(elementsLine);
    std::vector<Backpack::SimpleItem> simpleItems(
        (std::istream_iterator<Backpack::SimpleItem>(elements)),
        std::istream_iterator<Backpack::SimpleItem>());

    for (auto &si : simpleItems) {
      last.items.push_back(Item("Item", si.mass, si.mass));
    }

    last.packed.assign(last.items.size() + 1, std::vector<int>(cap + 1, 0));
    last.packed_idx.assign(last.items.size() + 1,
                           std::vector<int>(cap + 1, -1));
  }
  return backpacks;
}

std::vector<int> Backpack::backtrack() {
  int n = items.size();

  std::vector<int> selectedWeights;
  int i = n;
  int j = capacity;

  while (i > 0 && j > 0) {
    if (packed_idx[i][j] == i - 1) {
      selectedWeights.push_back(items[i - 1].mass);
      j -= items[i - 1].mass;
    }
    i--;
  }

  std::reverse(selectedWeights.begin(), selectedWeights.end());

  return selectedWeights;
  void load_file(const std::string &filepath);
}
