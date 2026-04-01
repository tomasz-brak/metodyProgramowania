#include <functional>
class Edge {
public:
  int in;
  int out;
  Edge(int in_param, int out_param) : in(in_param), out(out_param) {}
  bool operator==(const Edge &other) const {
    return other.in == this->in && other.out == this->out;
  }
};

struct EdgeHash {
  std::size_t operator()(const Edge &edge) const {
    // A simple way to combine hashes
    return std::hash<int>{}(edge.in) ^ (std::hash<int>{}(edge.out) << 1);
  }
};
