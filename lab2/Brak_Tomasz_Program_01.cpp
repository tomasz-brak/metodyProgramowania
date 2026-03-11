#include "Engine.h"
#include "EngineSEB.h"
#include "Files.h"
#include "array.h"
#include "lib/EngineSEAK.h"
#include <fstream>
#include <string>

constexpr int ENGINE_COUNT_PER_FILE = 81;

int main() {
  std::ofstream outputSEAK = promptOpenFile<std::ofstream>(
      "Provide output for path for SEAK data: ", isTxt);

  std::ofstream outputSEB = promptOpenFile<std::ofstream>(
      "Provide output for path for SEB data: ", isTxt);

  std::ifstream sSEAK = promptOpenFile<std::ifstream>(
      "Provide path for seak data:", isTxt,
      "/home/tomasz/dev/metodyProgramowaia/lab2/data/"
      "silnikSEAK/siatka_SEAK.txt");
  std::ifstream sSEB = promptOpenFile<std::ifstream>(
      "Provide path for seb data:", isTxt,
      "/home/tomasz/dev/metodyProgramowaia/lab2/data/silnikSEB/siatkaSEB.txt");

  std::ifstream sUnkSEAK = promptOpenFile<std::ifstream>(
      "Provide path for unknown SEAK:", isTxt,
      "/home/tomasz/dev/metodyProgramowaia/lab2/data/silnikSEAK/nmSEAK.txt");
  std::ifstream sUnkSEB = promptOpenFile<std::ifstream>(
      "Provide path for unknown SEB:", isTxt,
      "/home/tomasz/dev/metodyProgramowaia/lab2/data/silnikSEB/nmSEB.txt");

  Array<EngineSEAK, ENGINE_COUNT_PER_FILE> enginesSEAK;
  Array<EngineSEB, ENGINE_COUNT_PER_FILE> enginesSEB;

  EngineSEAK unknownSEAK;
  EngineSEB unknownSEB;

  for (int i = 0; i < ENGINE_COUNT_PER_FILE; ++i) {
    enginesSEAK[i].load(sSEAK);
    enginesSEB[i].load(sSEB);
  }
  unknownSEAK.load(sUnkSEAK);
  unknownSEB.load(sUnkSEB);

  Engine::findMatch(outputSEAK, unknownSEAK, enginesSEAK);
  Engine::findMatch(outputSEB, unknownSEB, enginesSEB);

  return 0;
}
