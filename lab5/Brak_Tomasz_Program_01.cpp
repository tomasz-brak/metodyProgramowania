#include "./lib/logging.h"
#include "./src/graph.h"
#include "Files.h"
#include <algorithm>
#include <fstream>
#include <unordered_map>
#include <unordered_set>
#include <vector>

std::unordered_set<Edge, EdgeHash> edges;
std::unordered_map<int, std::vector<Edge>> toVertex;
std::unordered_map<int, std::vector<Edge>> fromVertex;

std::vector<int> vertices;

std::vector<std::vector<int>> incidenceMatrix;

int main() {
  Logger::info("Hello World!");
  std::ifstream inFile = promptOpenFile<std::ifstream>(
      "Open input file: ", [](std::string _) { return true; });
  std::ofstream outFile = promptOpenFile<std::ofstream>(
      "Provide a path for output", [](std::string _) { return true; });
  int k, in, out;
  inFile >> k;
  for (int i = 0; i < k; ++i) {
    inFile >> in >> out;
    auto found = std::find_if(vertices.begin(), vertices.end(),
                              [in](int &element) { return element == in; });
    if (found == vertices.end()) {
      vertices.emplace_back(in);
    }
    found = std::find_if(vertices.begin(), vertices.end(),
                         [out](int &element) { return element == out; });
    if (found == vertices.end()) {
      vertices.emplace_back(out);
    }
    Edge e = Edge(in, out);
    toVertex[in].push_back(e);
    fromVertex[out].push_back(e);
    edges.emplace(e);
  }
  outFile << "Macierz incydencji:\n";
  for (auto &vertex : vertices) {
    for (auto &edge : edges) {
      if (edge.in == vertex) {
        outFile << "1 ";
      } else if (edge.out == vertex) {
        outFile << "-1 ";

      } else {
        outFile << "0 ";
      }
    }
    outFile << "\n";
  }
  outFile << "Correct way neighbour list:\n";
  for (auto &[vertex, edges] : toVertex) {
    outFile << "V" << vertex << ": ";
    for (auto &edge : edges) {
      outFile << edge.out;
    }
    outFile << "\n";
  }
  outFile << "Incorrect way neighbour list:\n";
  for (auto &[vertex, edges] : fromVertex) {
    outFile << "V" << vertex << ": ";
    for (auto &edge : edges) {
      outFile << edge.out;
    }
    outFile << "\n";
  }
  outFile.close();
  return 0;
}
