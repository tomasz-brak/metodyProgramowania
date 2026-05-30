#pragma once
#include "error.h"
#include "logging.h"
#include <concepts>
#include <memory>
#include <new>
#include <queue>
#include <vector>
template <class T> using vec = std::vector<T>;

template <typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

/**
 * @brief Represents an input data point for the Huffman encoding algorithm
 *
 * @tparam T Any numeric type representing the frequency of the letters
 */
template <Numeric T> struct datapoint {
  char letter;
  T probability;
};

/**
 * @class resultDatapoint
 * @brief Represents a result datapoint
 * Contains the codeword for each letter given as an input
 */
struct resultDatapoint {
  char letter;
  std::string codeword = "";
};

/**
 * @brief Base class expressing any node in the binary tree of the algorithm
 */
template <Numeric T> struct anyNode {
  T value{};
  std::string codeword;
  virtual ~anyNode() = default;
  virtual bool isLeaf() const { return false; }
};

/**
 * @brief Leaf of the tree representing a charachter
 *
 * @param ch charachter
 * @param v frequency/probability of a charachter
 */
template <Numeric T> struct valueNode : public anyNode<T> {
  char c{};
  valueNode(char ch, T v) {
    this->value = v;
    c = ch;
  }
  bool isLeaf() const override { return true; }
};

/**
 * @brief An split in a tree, each split has two children left gets assigned
 * number 1 and right number 0
 *
 * @param a unique_ptr to the first node
 * @param b unique_ptr to the second node
 */
template <Numeric T> struct splitNode : public anyNode<T> {
  std::unique_ptr<anyNode<T>> node1{};
  std::unique_ptr<anyNode<T>> node2{};
  splitNode(std::unique_ptr<anyNode<T>> a, std::unique_ptr<anyNode<T>> b)
      : node1(std::move(a)), node2(std::move(b)) {
    this->value = node1->value + node2->value;
  }
};

/**
 * @brief Comparison for the priority_queue
 *
 * @param a first node
 * @param b second node
 * @return to sort in accending order if a > b
 */
template <Numeric T> struct Compare {
  bool operator()(const std::unique_ptr<anyNode<T>> &a,
                  const std::unique_ptr<anyNode<T>> &b) const {
    if (!a || !b) {
      throw NullComparisonError();
    }
    return a->value > b->value;
  }
};

/**
 * @brief Tree traversal and binary code word genarator
 * - Performs DFS from the root node
 * - When going LEFT adds a 1 to the parents codeword
 * - When going RIGHT adds a 0 to the parents codeword
 * - If a leaf is found it gets added to \t bottom
 * @param root currently processed subtree
 * @param bottom out-parameter collects all the leaves with codewords assigned
 */
template <Numeric T>
void evaluate(anyNode<T> *root, vec<valueNode<T> *> &bottom) {
  if (root->isLeaf()) {
    bottom.push_back(static_cast<valueNode<T> *>(root));
    return;
  }
  auto st = static_cast<splitNode<T> *>(root);

  st->node1->codeword = root->codeword + "1";
  st->node2->codeword = root->codeword + "0";
  evaluate(st->node1.get(), bottom);
  evaluate(st->node2.get(), bottom);
}

/**
 * @brief Generates Huffman codes according to the letter frequency given
 * Uses a queue to convert all the data points in to a tree structure.
 *
 * Elements on the queue are merged together until only one remains. Joint
 * elements end up on the queue. Queue is sorted from lowest to highest letter
 * frequency/probability. This way 2 first elements (with lowest values) will
 * always be on the bottom.
 *
 * @param datapoints Vector containing the entry datapoints
 * @return result of pairings of a char to their respective codeword
 */
template <Numeric T>
vec<resultDatapoint> solution(const vec<datapoint<T>> &datapoints) {
  std::priority_queue<std::unique_ptr<anyNode<T>>,
                      vec<std::unique_ptr<anyNode<T>>>, Compare<T>>
      queue;

  for (auto const &dp : datapoints) {
    try {
      queue.push(std::make_unique<valueNode<T>>(dp.letter, dp.probability));
    } catch (std::bad_alloc) {
      Logger::critical(
          "Memory allocation for the tree structure failed! Exiting...");
    }
  }
  while (queue.size() > 1) {
    // const_cast to cast the const enforced by the priority_queue to move the
    // element, since move breakes the element in the queue it leaves the queue
    // in a broken state. But the moved-from element is removed right after.
    auto node1 =
        std::move(const_cast<std::unique_ptr<anyNode<T>> &>(queue.top()));
    queue.pop();
    auto node2 =
        std::move(const_cast<std::unique_ptr<anyNode<T>> &>(queue.top()));
    queue.pop();
    queue.push(
        std::make_unique<splitNode<T>>(std::move(node1), std::move(node2)));
  }
  auto root = std::move(const_cast<std::unique_ptr<anyNode<T>> &>(queue.top()));
  queue.pop();
  vec<valueNode<T> *> result;
  evaluate(root.get(), result);
  vec<resultDatapoint> out;
  for (auto const &dp : result) {
    out.push_back(resultDatapoint{dp->c, dp->codeword});
  }

  return out;
}
