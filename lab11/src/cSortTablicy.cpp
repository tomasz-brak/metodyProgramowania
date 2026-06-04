#include "cSortTablicy.h"
#include <algorithm>
#include <functional>
#include <iostream>
#include <map>
#include <print>
#include <random>


// dane wprowadzane przez uzytkownika
void cSortTablicy::wprowadzZKlawiatury() {
  int rozmiar;
  std::print("Podaj liczbe elementow do wpisania: ");
  std::cin >> rozmiar;

  tablica.data.clear();
  std::println("Wpisz {} elementow", rozmiar);
  for (int i = 0; i < rozmiar; ++i) {
    int element;
    std::cin >> element;
    tablica.data.push_back(element);
  }
}

// Generator liczb pseudolosowych
void cSortTablicy::generujLosowe(int rozmiar) {
  tablica.data.clear();

  std::random_device rd;
  std::mt19937 mt(rd());
  std::uniform_real_distribution<double> dis(-1.0, 1.0);

  for (int i = 0; i < rozmiar; ++i) {
    double x = dis(mt);
    tablica.data.push_back(
        static_cast<int>(x * 100000)); // static cast jest po to zeby zamienic
                                       // np 5645.0 na 5645 (double nna int)
  }
}

// Tablica posortowana rosnąco
void cSortTablicy::generujUporzadkowane(int rozmiar) {
  tablica.data.clear();
  for (int i = 0; i < rozmiar; ++i) {
    tablica.data.push_back(i);
  }
}

// Tablica posortowana malejąco
void cSortTablicy::generujOdwrotnieUporzadkowane(int rozmiar) {
  tablica.data.clear();
  for (int i = rozmiar; i > 0; --i) {
    tablica.data.push_back(i);
  }
}

// Tablica prawie posortowana (10% na złych pozycjach)
void cSortTablicy::generujPrawiePosortowane(int rozmiar) {
  generujUporzadkowane(rozmiar);
  std::random_device rd;
  std::mt19937 mt(rd());
  int liczbaZamian = static_cast<int>(rozmiar * 0.05);
  if (liczbaZamian == 0 && rozmiar > 1)
    liczbaZamian = 1;

  std::uniform_int_distribution<int> dis(0, rozmiar - 1);
  for (int i = 0; i < liczbaZamian; ++i) {
    std::swap(tablica.data[dis(mt)], tablica.data[dis(mt)]);
  }
}

// 3. Wybór i ilustracja działania metod sortowania
void cSortTablicy::showResults(const std::string &rodzajDanych) {
  int rozmiar = tablica.data.size();
  std::println("\n==================================================");
  std::println("TEST DLA: Rozmiar = {}, Rodzaj danych = {}", rozmiar,
               rodzajDanych);
  std::println("==================================================");

  const std::map<std::string, std::function<void(cTablica *, testResult &)>>
      sorters = {{"Babelkowe", &cTablica::sortbobel},
                 {"Wachadlowe", &cTablica::sortwach},
                 {"Lomuto QuickSort", &cTablica::sortlomuto},
                 {"Hoare QuickSort", &cTablica::sorthoare},
                 {"Przez kopcowanie", &cTablica::sortkopcowanie}};

  for (auto &[nazwa, algorytm] : sorters) {
    cTablica kopiaTablicy = tablica;

    // BOBELKOWE SIE ZA DLUGO LICZY, PRZY ODDAWANIU MOZNA TO USUNAC
    if (rozmiar >= 10000 && (nazwa == "Babelkowe" || nazwa == "Wachadlowe")) {
      continue;
    }
    kopiaTablicy.test(algorytm, nazwa);
  }
}
