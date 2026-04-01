#include "lib/Files.h"
#include "lib/containers/list.h"
#include "lib/logging.h"
#include "src/DirectionalList.h"
#include "src/graph.h"
#include <fstream>
#include <iostream>
#include <list>
int main() {
  Logger::info("Hello World!");

  std::ifstream inFile = promptOpenFile<std::ifstream>(
      "Open input file: ", [](std::string) { return true; });

  std::ofstream outFile = promptOpenFile<std::ofstream>(
      "Provide a path for output ", [](std::string) { return true; });

  if (!inFile.is_open() || !outFile.is_open()) {
    return 1;
  }
  outFile << "=== Using a std::list === ";
  int k, in, out;
  Graph<std::list> graph2;
  inFile.clear();
  inFile.seekg(0, std::ios::beg);
  if (!(inFile >> k))
    return 0;

  for (int i = 0; i < k; ++i) {
    if (inFile >> in >> out) {
      graph2.addEdge(in, out);
    }
  }

  graph2.writeIncidenceList(outFile);

  outFile << "=== Using a one way list === ";
  Graph<DirectionalList> graph3;
  inFile.clear();
  inFile.seekg(0, std::ios::beg);
  if (!(inFile >> k))
    return 0;

  for (int i = 0; i < k; ++i) {
    if (inFile >> in >> out) {
      graph3.addEdge(in, out);
    }
  }

  graph3.writeIncidenceList(outFile);

  outFile.close();
  inFile.close();

  return 0;
}
