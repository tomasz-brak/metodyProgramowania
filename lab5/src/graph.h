#pragma once

#include <algorithm>
#include <concepts>
#include <iostream>
#include <ranges>
#include <unordered_map>
#include <unordered_set>
#include <vector>

/**
 * @class Edge
 * @brief Define an edge of a graph
 * Defines operator == for the use in a hashset
 */
struct Edge {
  int in;
  int out;

  bool operator==(const Edge &other) const = default;
};

/**
 * @class EdgeHash
 * @brief Hash for the hashset of the edge object
 */
struct EdgeHash {
  std::size_t operator()(const Edge &edge) const {
    return std::hash<int>{}(edge.in) ^ (std::hash<int>{}(edge.out) << 1);
  }
};

/**
 * @brief Class for managing the structure of a graph can be instantiated with
 * diffrent underlying storage structures for the elements
 *
 */
template <template <typename...> class Container = std::vector> class Graph {
public:
  using EdgeContainer = Container<Edge>;
  using VertexContainer = Container<int>;

  /**
   * @brief Add an edge to the graph given
   *
   * @param in the vertex the edge comes from
   * @param out the vertex the element goes to
   */
  inline void addEdge(int in, int out) {
    addVertexIfNew(in);
    addVertexIfNew(out);

    Edge e{in, out};
    if (edges.insert(e).second) {
      agnosticAppend(successorVertices[in], e);
      agnosticAppend(predecessorVertices[out], e);
    }
  }

  /**
   * @brief Writes an incidence matrix to a stream
   *
   * @param out stream to write to
   */
  inline void writeIncidenceMatrix(std::ostream &out) const {
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

  /**
   * @brief Write a list of sucessors for each vertex
   *
   * @param out stream to write to
   */
  inline void writeSuccessorList(std::ostream &out) const {
    out << "Successor list:\n";
    for (const auto &[vertex, edgeList] : successorVertices) {
      out << "V" << vertex << ": ";
      for (const auto &edge : edgeList)
        out << edge.out << " ";
      out << "\n";
    }
  }

  /**
   * @brief Write a list of predecessors for each vertex
   *
   * @param out stream tot write to
   */
  inline void writePredecessorList(std::ostream &out) const {
    out << "Predecessor list:\n";
    for (const auto &[vertex, edgeList] : predecessorVertices) {
      out << "V" << vertex << ": ";
      for (const auto &edge : edgeList) {
        out << edge.in << " ";
      }
      out << "\n";
    }
  }

  /**
   * @brief Write an incidence list (list of connections between each vertex)
   *
   * @param out stream to write to
   */
  inline void writeIncidenceList(std::ostream &out) const {
    for (int v : vertices) {
      auto incident = edges | std::views::filter([v](const Edge &e) {
                        return e.in == v || e.out == v;
                      });

      out << "Vertex " << v << ": ";
      int degree = 0;
      for (const auto &edge : incident) {
        out << "{" << edge.in << ", " << edge.out << "} ";
        degree++;
      }
      out << "\n   -> Degree: " << degree << "\n";
    }
  }

private:
  /**
   * @brief Add a vertex to the map of vertieces if it is not present
   * @param v vertex id
   */
  inline void addVertexIfNew(int v) {
    if (std::find(vertices.begin(), vertices.end(), v) == vertices.end()) {
      agnosticAppend(vertices, v);
    }
  }

  /**
   * @brief append to a @ref T container a value
   *
   * @tparam C Container type
   * @tparam T Element type
   * @param container container to add to
   * @param value value to add (forwarded to the continaer method)
   */
  template <typename C, typename T>
  static constexpr inline void agnosticAppend(C &container, T &&value) {
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
