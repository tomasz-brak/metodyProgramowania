#pragma once
#include "backpack.h"
#include <memory>
#include <string>
#include <vector>

template <class T> using vec = std::vector<T>;
template <class T> using box = std::unique_ptr<T>;

class Node {
public:
  int item_id = 0;
  int price = 0;
  int weight = 0;
  float pw_ratio = 0.0;
};

class Solution {
public:
  box<Backpack> backpack = std::make_unique<Backpack>();
  vec<Node> solve();
  void load_file(const std::string &filepath);
};
