#include <any>
#include <print>
#include <queue>
#include <ranges>
#include <vector>

template <class T> using vec = std::vector<T>;

template <typename T> struct datapoint {
  char letter;
  T probability;
};

struct resultDatapoint {
  char letter;
  std::string codeword = "";
};

template <typename T> struct anyNode {
  T value{};
  std::string codeword;
  virtual ~anyNode() = default;
  virtual bool isLeaf() const { return false; }
};

template <typename T> struct valueNode : public anyNode<T> {
  char c{};
  valueNode(char ch, T v) {
    this->value = v;
    c = ch;
  }
  bool isLeaf() const override { return true; }
};

template <typename T> struct splitNode : public anyNode<T> {
  anyNode<T> *node1{};
  anyNode<T> *node2{};
  splitNode(anyNode<T> *a, anyNode<T> *b) {
    node1 = a;
    node2 = b;
    this->value = a->value + b->value;
  }
};

template <typename T> struct Compare {
  bool operator()(const anyNode<T> *a, const anyNode<T> *b) const {
    return a->value > b->value;
  }
};

template <typename T>
void evaluate(anyNode<T> *root, vec<valueNode<T> *> &bottom) {
  if (root->isLeaf()) {
    bottom.push_back(static_cast<valueNode<T> *>(root));
    return;
  }
  auto st = static_cast<splitNode<T> *>(root);

  st->node1->codeword = root->codeword + "1";
  st->node2->codeword = root->codeword + "0";

  evaluate(st->node1, bottom);
  evaluate(st->node2, bottom);
}

template <typename T>
vec<resultDatapoint> solution(const vec<datapoint<T>> &datapoints) {
  std::priority_queue<anyNode<T> *, std::vector<anyNode<T> *>, Compare<T>>
      queue;

  for (auto const &dp : datapoints) {
    queue.push(new valueNode<T>(dp.letter, dp.probability));
  }
  while (queue.size() > 1) {
    anyNode<T> *node1 = queue.top();
    queue.pop();
    anyNode<T> *node2 = queue.top();
    queue.pop();
    queue.push(new splitNode<T>(node1, node2));
  }
  anyNode<T> *root = queue.top();
  vec<valueNode<T> *> result;
  evaluate(root, result);
  vec<resultDatapoint> out;
  for (auto const &dp : result) {
    out.push_back(resultDatapoint{dp->c, dp->codeword});
  }

  (void)root;
  return out;
}

int main() {
  vec<datapoint<int>> data = {
      {'A', 12}, {'B', 7}, {'I', 18}, {'M', 10}, {'S', 9}, {'X', 5}, {'Z', 2},
  };

  auto res = solution(data);
  for (auto const &r : res) {
    std::print("{} -> {}\n", r.letter, r.codeword);
  }

  std::print("Dataset 2:\n");

  vec<datapoint<float>> d2 = {{'c', 0.11}, {'e', 0.22}, {'i', 0.16},
                              {'r', 0.12}, {'s', 0.15}, {'t', 0.10},
                              {'x', 0.14}};

  auto res2 = solution(d2);
  for (auto const &r : res2) {
    std::print("{} -> {}\n", r.letter, r.codeword);
  }
}
