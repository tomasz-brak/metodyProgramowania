#pragma once
#include "cTablica.h"
#include <string>

class cSortTablicy {
public:
  cTablica tablica;

  void wprowadzZKlawiatury();
  void generujLosowe(int rozmiar);
  void generujUporzadkowane(int rozmiar);
  void generujOdwrotnieUporzadkowane(int rozmiar);
  void generujPrawiePosortowane(int rozmiar);
  void showResults(const std::string &rodzajDanych);
};
