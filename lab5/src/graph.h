#pragma once

#include <algorithm>
#include <concepts>
#include <iostream>
#include <ranges>
#include <unordered_map>
#include <unordered_set>
#include <vector>

struct Edge {
  int in;
  int out;

  bool operator==(const Edge &other) const = default;
};

/**
 * @class EdgeHash
 * @brief Hash for the unordered_map for the edge object
 */
struct EdgeHash {
  std::size_t operator()(const Edge &edge) const {
    return std::hash<int>{}(edge.in) ^ (std::hash<int>{}(edge.out) << 1);
  }
};

template <typename C, typename T>
concept AppendableContainer = requires(C &c, T v) {
  { c.push_back(v) } -> std::same_as<void>;
} || requires(C &c, T v) {
  { c.append(v) } -> std::same_as<void>;
};

template <template <typename...> class Container = std::vector> class Graph {
public:
  using EdgeContainer = Container<Edge>;
  using VertexContainer = Container<int>;

  void addEdge(int in, int out) {
    addVertexIfNew(in);
    addVertexIfNew(out);

    Edge e{in, out};
    if (edges.insert(e).second) { // Only add to lists if edge is actually new
      agnosticAppend(successorVertices[in], e);
      agnosticAppend(predecessorVertices[out], e);
    }
  }

  void writeIncidenceMatrix(std::ostream &out) const {
    out << "Incidence Matrix:\n";
    for (int vertex : vertices) {
      for (const auto &edge : edges) {
        if (edge.in == vertex)
          out << "-1 ";
        else if (edge.out == vertex)
          out << "1 ";
        else
          out << "0 ";
      }
      out << "\n";
    }
  }

  void writeSuccessorList(std::ostream &out) const {
    out << "Successor list:\n";
    for (const auto &[vertex, edgeList] : successorVertices) {
      out << "V" << vertex << ": ";
      for (const auto &edge : edgeList)
        out << edge.out << " ";
      out << "\n";
    }
  }

  void writeIncidenceList(std::ostream &out) const {
    out << "\n--- Incidence List & Vertex Degrees ---\n";
    for (int v : vertices) {
      // Using ranges to simplify finding incident edges
      auto incident = edges | std::views::filter([v](const Edge &e) {
                        return e.in == v || e.out == v;
                      });

      out << "Vertex " << v << " (Incident edges): ";
      int degree = 0;
      for (const auto &edge : incident) {
        out << "{" << edge.in << ", " << edge.out << "} ";
        degree++;
      }
      out << "\n   -> Degree: " << degree << "\n";
    }
  }

private:
  void addVertexIfNew(int v) {
    if (std::find(vertices.begin(), vertices.end(), v) == vertices.end()) {
      agnosticAppend(vertices, v);
    }
  }

  template <typename C, typename T>
  static void agnosticAppend(C &container, T &&value) {
    if constexpr (requires { container.push_back(std::forward<T>(value)); }) {
      container.push_back(std::forward<T>(value));
    } else {
      container.append(std::forward<T>(value));
    }
  }

  VertexContainer vertices;
  std::unordered_set<Edge, EdgeHash> edges;
  std::unordered_map<int, EdgeContainer> successorVertices;
  std::unordered_map<int, EdgeContainer> predecessorVertices;
};
