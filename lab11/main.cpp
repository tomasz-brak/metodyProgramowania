#include "src/cSortTablicy.h"
#include <iostream>
#include <print>
#include <string>
#include <vector>
template <typename T> using vec = std::vector<T>;

int main() {
  cSortTablicy manager;
  std::string wybor;

  while (true) {
    std::println("1. Wprowadz wlasne dane z klawiatury i przetestuj");
    std::println("2. Uruchom automatyczny test zadanych dlugosci");
    std::print("Wybierz opcje: ");
    std::cin >> wybor;

    if (wybor == "1") {
      manager.wprowadzZKlawiatury();
      manager.showResults("Dane reczne z klawiatury");
    } else if (wybor == "2") {
      std::vector<int> rozmiary = {100, 1000, 100000};

      for (int r : rozmiary) {
        manager.generujLosowe(r);
        manager.showResults("Losowe");

        manager.generujUporzadkowane(r);
        manager.showResults("Uporzadkowane rosnaco");

        manager.generujOdwrotnieUporzadkowane(r);
        manager.showResults("Uporzadkowane malejaco");

        manager.generujPrawiePosortowane(r);
        manager.showResults("Prawie posortowane (10% bledow)");
      }

      break;
    } else {
      break;
    }
  }
  return 0;
}
