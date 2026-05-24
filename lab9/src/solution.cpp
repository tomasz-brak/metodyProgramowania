#include "solution.h"
#include "../lib/logging.h"
#include "backpack.h"
#include <algorithm>
#include <fstream>
#include <memory>
#include <string>
void Solution::load_file(const std::string &filepath) {
  std::ifstream file(filepath);
  this->backpack = std::make_shared<Backpack>(file);
  file.close();
}

/**
 * @brief Calculates the *upper potential profit* for a node.
 *
 * Calculates heuristicly the best-case profit scenario the branch from \p node.
 * Adding all remaining items possible to fill the backpack. If item would not
 * fit it adds a fraction of it.
 *
 * @note Value is only calculated for tree prunning purposes. If bound(node) <
 * current `max_profit` the node is deemed inefficient and pruned.
 *
 * @param node Node to be evaluated.
 * @param backpack Backpack config and its content.
 * @return maximum theoretical profit for the \p node branch.
 */
float bound(rc<Node> node, const rc<Backpack> backpack) {
  if (node->weight > backpack->capacity)
    return 0;

  float profit_bound = node->profit;
  int toweight = node->weight;

  size_t j = node->depth;
  size_t num_items = backpack->items.size();
  int W = backpack->capacity;

  while (j < num_items && toweight + backpack->items[j].mass <= W) {
    toweight += backpack->items[j].mass;
    profit_bound += backpack->items[j].price;
    j++;
  }

  if (j < num_items) {
    float remaining_capacity = W - toweight;
    float price_per_unit =
        (float)backpack->items[j].price / backpack->items[j].mass;
    profit_bound += remaining_capacity * price_per_unit;
  }

  return profit_bound;
}

/**
 * @brief Uses a greedy approach to quickly calculate a baseline profit.
 *
 * Iterates through items and takes them until capacity is reached.
 *
 * This allows for pruning inefficient branches right from the beginning.
 *
 * @param backpack Backpack with **presorted items**
 * @return a profit achieved by the greedy alghorithm.
 */
int getGreedyBaseline(const rc<Backpack> &backpack) {
  int current_weight = 0;
  int current_profit = 0;
  for (const auto &item : backpack->items) {
    if (current_weight + item.mass <= backpack->capacity) {
      current_weight += item.mass;
      current_profit += item.price;
    }
  }
  return current_profit;
}

/**
 * @brief Generates a decision tree
 *
 * Processes items, for each it creates a decision node either
 *  TAKES:
 *    - item added, weight + profit increases.
 *  LEAVES:
 *    - item left behind, profit + weight from the parent.
 *
 * After creation for each node an evaluation happens and for each node whose
 * bound is vaiable (being grater then maxProfit) is left for the next tree
 * level to process. If node is not viable it is PRUNED and removed from the
 * tree.
 */
void Solution::createTree() {
  this->root = std::make_shared<Node>();
  this->root->weight = 0;
  this->root->profit = 0;
  this->root->depth = 0;
  this->root->item_id = "Root";
  this->root->bound = bound(this->root, backpack);

  vec<rc<Node>> nextLevel;
  vec<rc<Node>> level = {this->root};

  int maxProfit = getGreedyBaseline(this->backpack);
  int depth = 0;

  Logger::debug("Started tree creation...");
  Logger::debug("capacity: {} max_profit: {}", backpack->capacity, maxProfit);

  for (Backpack::Item item : this->backpack->items) {
    nextLevel.clear();
    Logger::debug("[depth: {}] Item: {} (Price: {}, Mass: {})", depth,
                  item.name, item.price, item.mass);
    Logger::debug("Current level active nodes count: {}", level.size());

    for (auto parentNode : level) {
      parentNode->takes = nullptr;
      parentNode->leaves = nullptr;

      parentNode->takes = std::make_shared<Node>();
      parentNode->takes->parent = parentNode;
      parentNode->takes->item_id = item.name;
      parentNode->takes->profit = parentNode->profit + item.price;
      parentNode->takes->weight = parentNode->weight + item.mass;
      parentNode->takes->depth = depth + 1;
      parentNode->takes->bound = bound(parentNode->takes, backpack);

      Logger::debug("  -> [TAKE] Node '{}': Weight={}/{}, Profit={}, Bound={}",
                    parentNode->takes->item_id, parentNode->takes->weight,
                    backpack->capacity, parentNode->takes->profit,
                    parentNode->takes->bound);

      if (parentNode->takes->weight <= backpack->capacity &&
          parentNode->takes->profit > maxProfit) {
        int oldProfit = maxProfit;
        maxProfit = parentNode->takes->profit;
        Logger::debug("     maxProfit changed: {} -> {}", oldProfit, maxProfit);
      }

      parentNode->leaves = std::make_shared<Node>();
      parentNode->leaves->parent = parentNode;
      parentNode->leaves->item_id = "Left " + item.name;
      parentNode->leaves->profit = parentNode->profit;
      parentNode->leaves->weight = parentNode->weight;
      parentNode->leaves->depth = depth + 1;
      parentNode->leaves->bound = bound(parentNode->leaves, backpack);

      Logger::debug("  -> [LEAVE] Node '{}': Weight={}/{}, Profit={}, Bound={}",
                    parentNode->leaves->item_id, parentNode->leaves->weight,
                    backpack->capacity, parentNode->leaves->profit,
                    parentNode->leaves->bound);

      if (parentNode->leaves->weight <= backpack->capacity &&
          parentNode->leaves->profit > maxProfit) {
        int oldProfit = maxProfit;
        maxProfit = parentNode->leaves->profit;
        Logger::debug("     maxProfit changed: {} -> {} **", oldProfit,
                      maxProfit);
      }

      if (parentNode->takes->weight <= backpack->capacity &&
          parentNode->takes->bound >= maxProfit) {
        nextLevel.push_back(parentNode->takes);
      } else {
        Logger::debug("     PRUNED TAKE node '{}'", parentNode->takes->item_id);
        parentNode->takes = nullptr;
      }

      if (parentNode->leaves->weight <= backpack->capacity &&
          parentNode->leaves->bound >= maxProfit) {
        nextLevel.push_back(parentNode->leaves);
      } else {
        Logger::debug("     PRUNED LEAVE node '{}'",
                      parentNode->leaves->item_id);
        parentNode->leaves = nullptr;
      }
    }

    if (nextLevel.empty()) {
      Logger::debug("Next level empty, finished");
      break;
    }
    level = nextLevel;
    depth++;
  }
  Logger::debug("Tree Creation Finished");
}

/**
 * @brief Finds the best solution out of the tree
 * Starting at ROOT it goes down each possible path, if it encounters a node
 * with both TAKES and LEAVES pointers = nullptr it compares it to max profit.
 *
 * If it is bigger then maxProfit it is a current best leaf.
 * @param node the node being inspected
 * @param maxProfit current best profit
 * @param bestLeaf current best candidate
 */
void harvestBestLeaf(rc<Node> node, int &maxProfit, rc<Node> &bestLeaf) {
  if (node == nullptr)
    return;

  Logger::debug("[Harvest] Inspecting Node: '{}', Profit: {}, Weight: {}, "
                "Takes: {}, Leaves: {}",
                node->item_id, node->profit, node->weight,
                node->takes ? "Valid" : "Null",
                node->leaves ? "Valid" : "Null");

  if (node->takes == nullptr && node->leaves == nullptr) {
    Logger::debug(
        "  Found structural leaf node '{}'. Profit: {}, Current maxProfit: {}",
        node->item_id, node->profit, maxProfit);
    if (node->profit >= maxProfit) {
      maxProfit = node->profit;
      bestLeaf = node;
      Logger::debug("  >> New bestLeaf assigned: '{}' with Profit {} <<",
                    node->item_id, maxProfit);
    }
    return;
  }

  harvestBestLeaf(node->takes, maxProfit, bestLeaf);
  harvestBestLeaf(node->leaves, maxProfit, bestLeaf);
}

/**
 * @brief Prepairs the solution for the problem.
 *
 * Ensures the data in the backpack is sorted corretly then assembles the
 * solution tree.
 * After the solution tree is established it performs DFS to find the best
 * possible combination of nodes
 *
 * **Backtracking** - after finding an optimal solution the algorithm backtracks
 * using the weak_ptr parent pointer to establish a result vector
 *
 * @return result vector containing nodes yielding an optimal solution
 */
vec<Node> Solution::solve() {
  std::sort(backpack->items.begin(), backpack->items.end(),
            [](const Backpack::Item &item1, const Backpack::Item &item2) {
              float ratio1 = (float)item1.price / item1.mass;
              float ratio2 = (float)item2.price / item2.mass;
              return ratio1 > ratio2;
            });

  createTree();

  int maxProfit = 0;
  rc<Node> bestLeaf = nullptr;

  Logger::debug("--- Starting Harvest Step ---");
  harvestBestLeaf(this->root, maxProfit, bestLeaf);
  Logger::debug("--- Harvest Step Complete ---");

  if (bestLeaf != nullptr) {
    Logger::debug("Winning leaf path traces down to Node ID: '{}' with final "
                  "total profit: {} and total weight: {}",
                  bestLeaf->item_id, bestLeaf->profit, bestLeaf->weight);
  } else {
    Logger::debug("CRITICAL ERROR: No valid bestLeaf found!");
  }

  vec<Node> chosenItems;
  auto current = bestLeaf;

  while (current != nullptr) {
    auto parentOpt = current->parent.lock();
    if (parentOpt != nullptr) {
      if (current->profit > parentOpt->profit) {
        Node individualNode = *current;
        individualNode.profit = current->profit - parentOpt->profit;
        individualNode.weight = current->weight - parentOpt->weight;

        Logger::debug("[Backtrack] Captured item taken: {} (Item weight: {}, "
                      "Item profit: {})",
                      current->item_id, individualNode.weight,
                      individualNode.profit);
        chosenItems.push_back(individualNode);
      }
    }
    current = parentOpt;
  }

  std::reverse(chosenItems.begin(), chosenItems.end());
  return chosenItems;
}
