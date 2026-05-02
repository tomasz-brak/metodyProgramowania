#include "logging.h"
#include "src/random.h"
#include <iostream>
#include <string>

int main() {
  int method;

  Logger::debug("wybierz algorytm generujacy: (1 lcg) lub (2 acg)");
  if (!(std::cin >> method) || (method != 1 && method != 2)) {
    Logger::critical("Nieprawidlowy wybor algorytmu generujcego!");
    return 1;
  }

  int n;
  Logger::debug("Podaj liczbe elementow do wygenerowania: ");
  if (!(std::cin >> n) || n <= 0) {
    Logger::critical("Nieprawidlowa ilosc liczb do wygenerowania");
    return 1;
  }

  unsigned long long seed;
  Logger::debug("Podaj ziarno: ");
  if (!(std::cin >> seed)) {
    Logger::critical("Ziarno nie poprawne!");
    return 1;
  }

  std::string filename;
  Logger::debug("Podaj nazwe pliku: ");
  std::cin >> filename;

  if (method == 1) {
    long long Xmax;
    Logger::debug("Podaj liczbe maksymalna: ");
    if (!(std::cin >> Xmax) || Xmax <= 0) {
      Logger::critical("Liczba musi byc dodatnia");
      return 1;
    }

    unsigned long long m, a, c;
    calculateLCGParams(Xmax, m, a, c);

    generateLCG(seed, n, m, a, c, filename);

  } else {
    generateACG(seed, n, filename);
  }

  return 0;
}
