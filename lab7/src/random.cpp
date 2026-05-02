#include "random.h"
#include "./../lib/logging.h"
#include <fstream>
#include <numeric>
#include <vector>
void generateLCG(unsigned long long seed, int n, unsigned long long m,
                 unsigned long long a, unsigned long long c,
                 const std::string &filename) {
  std::ofstream file(filename);
  if (!file.is_open()) {
    Logger::error("Unable to open a file for writing!");
    return;
  }

  unsigned long long X = seed;

  for (int i = 0; i < n; ++i) {
    X = (a * X + c) % m;
    file << X << "\n";
  }
  file.close();
  Logger::info(
      "Pomyslnie wygerenerowano i zapisano {} liczb za pomocą generatora LCG",
      n);
}

void calculateLCGParams(long long Xmax, unsigned long long &m,
                        unsigned long long &a, unsigned long long &c) {
  m = Xmax + 1;

  long long temp = m;
  std::vector<long long> primeFactors;
  for (long long i = 2; i * i <= temp; ++i) {
    if (temp % i == 0) {
      primeFactors.push_back(i);
      while (temp % i == 0)
        temp /= i;
    }
  }
  if (temp > 1)
    primeFactors.push_back(temp);

  long long P = 1;
  for (long long factor : primeFactors) {
    P *= factor;
  }

  if (m % 4 == 0 && P % 4 != 0) {
    P *= 2;
  }

  long long k = (m > 100) ? (m / 2) / P : 1;
  if (k == 0)
    k = 1;
  a = 1 + k * P;

  c = 1;
  for (long long i = m / 2; i > 1; --i) {
    if (std::gcd(i, m) == 1) {
      c = i;
      break;
    }
  }
  Logger::info("Wyliczono parametry dla LCG: m={}, a={}, c={}", m, a, c);
}

void generateACG(unsigned long long seed, int n, const std::string &filename) {
  std::ofstream file(filename);
  if (!file.is_open()) {
    Logger::error("Unable to open a file for writing!");
    return;
  }

  const int j = 24;
  const int k = 55;
  unsigned long long m = 2147483648;

  std::vector<unsigned long long> state(k);

  unsigned long long X = seed;
  for (int i = 0; i < k; ++i) {
    X = (1103515245 * X + 12345) %
        m; // Dane podobne do tych uzywanych przez
           // https://github.com/gcc-mirror/gcc/blob/fee68dd1b484d5885abb110f1e8753f113db5713/libiberty/random.c#L236
    state[i] = X;
  }

  for (int i = 0; i < n; ++i) {
    unsigned long long next_val = (state[(i + k - j) % k] + state[i % k]) % m;
    file << next_val << "\n";
    state[i % k] = next_val;
  }
  file.close();
  Logger::info("Wygenerowano i zapisano liczby algorytmem ACG");
}
