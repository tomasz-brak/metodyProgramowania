#pragma once
#include "backpack.h"
#include <memory>
#include <string>
#include <vector>

template <class T> using vec = std::vector<T>;
template <class T> using box = std::unique_ptr<T>;
template <class T> using rc = std::shared_ptr<T>;
class Node {
public:
  std::weak_ptr<Node> parent;

  int depth = 0;
  std::string item_id;
  int profit = 0;
  int weight = 0;
  float bound = 0.0;
  rc<Node> takes;
  rc<Node> leaves;
};

class Solution {
private:
  void createTree();
  rc<Node> root;

public:
  rc<Backpack> backpack = std::make_unique<Backpack>();
  vec<Node> solve();
  void load_file(const std::string &filepath);
};
