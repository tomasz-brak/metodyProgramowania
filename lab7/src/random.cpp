#include "random.h"
#include "./../lib/logging.h"
#include <fstream>
#include <numeric>
#include <vector>
/**
 * @brief Calculates optimal LCG parameters
 *
 * Values need to adhere to principals:
 * 1. c and m are relatively prime: gcd(c, m) = 1.
 * 2. a - 1 is divisible by all prime factors of m.
 * 3. a - 1 is a multiple of 4 if m is a multiple of 4.
 *
 * @param Xmax The maximum desired value (m = Xmax + 1).
 * @param m Calculated modulus.
 * @param a Calculated multiplier.
 * @param c Calculated increment.
 */
void calculateLCGParams(long long Xmax, unsigned long long &m,
                        unsigned long long &a, unsigned long long &c) {
  m = Xmax + 1;

  /**
   * Find all prime factors of \ref m
   */
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

  /*
   * Multiply by all factors to find P. A number divisible by all prime factors
   * of m
   */
  long long P = std::accumulate(primeFactors.begin(), primeFactors.end(), 1,
                                std::multiplies<long long>());

  /*
   * Ensure that a-1 will be divisible by four if m is so.
   */
  if (m % 4 == 0 && P % 4 != 0) {
    P *= 2;
  }

  /*
   * a-1 is divisible by p. We target the middle by setting  k = (m/2) / P so
   * the data does not follow a slope.
   */
  long long k = (m / 2) / P;
  if (k == 0)
    k = 1;
  a = 1 + k * P;

  /*
   * We find such c that c and m are relatively prime i.e. their GCD = 1
   */
  c = 1;
  for (long long i = m / 2; i > 1; --i) {
    if (std::gcd(i, m) == 1) {
      c = i;
      break;
    }
  }
  Logger::info("Calculated LCG parms: m={}, a={}, c={}", m, a, c);
}

/**
 * @brief Computes the next value in a Linear Congruential Generator sequence
 *
 * Uses the standard recurrence formula: X_{n+1} = (a * X_n + c) mod m
 *
 * @param currentX The current value (X_n) in the sequence
 * @param a The multiplier
 * @param c The increment
 * @param m The modulus
 * @return The next pseudo-random value (X_{n+1})
 */
unsigned long long nextLCG(unsigned long long currentX, unsigned long long a,
                           unsigned long long c, unsigned long long m) {
  return (a * currentX + c) % m;
}

/**
 * @brief Generates a sequence of pseudo-random numbers using LCG and saves to a
 * file
 *
 * @param seed Starting value for the generator
 * @param n Number of values to generate
 * @param m The modulus (defines the range [0, m-1])
 * @param a The multiplier
 * @param c The increment
 * @param filename Path to the output file
 */
void generateLCG(unsigned long long seed, int n, unsigned long long m,
                 unsigned long long a, unsigned long long c,
                 const std::string &filename) {
  std::ofstream file(filename);
  if (!file.is_open()) {
    Logger::critical("Unable to open a file for writing!");
    return;
  }

  unsigned long long X = seed;
  for (int i = 0; i < n; ++i) {
    X = nextLCG(X, a, c, m);
    file << X << "\n";
  }
  file.close();
  Logger::info("Sucessfuly generated {} numbers with the LCG", n);
}

/**
 * @brief Generates numbers using an Additive Congruential Generator
 *
 * ACG formula: X_n = (X_{n-j} + X_{n-k}) mod m.
 *
 * @param seed The initial seed used to prime the LCG state.
 * @param n Total number of random values to output to the file.
 * @param filename Path to the output file.
 */
void generateACG(unsigned long long seed, int n, const std::string &filename) {
  std::ofstream file(filename);
  if (!file.is_open()) {
    Logger::error("Unable to open a file for writing!");
    return;
  }

  const int j = 24;
  const int k = 55;
  unsigned long long m_acg = 1000;

  /*
   * Calculate optimal parameters for a LCG generator and fill the vector with
   * LCG generated values
   */
  unsigned long long lcg_m, lcg_a, lcg_c;
  calculateLCGParams(m_acg - 1, lcg_m, lcg_a, lcg_c);

  std::vector<unsigned long long> current(k);

  unsigned long long X = seed;
  for (int i = 0; i < k; ++i) {
    X = nextLCG(X, lcg_a, lcg_c, lcg_m);
    current[i] = X;
  }

  for (int i = 0; i < n; ++i) {
    // in i+k-j we use the +j to avoid negative values from the % operator
    unsigned long long next_val =
        (current[(i + k - j) % k] + current[i % k]) % m_acg;
    file << next_val << "\n";
    current[i % k] = next_val;
  }

  file.close();
  Logger::info("Wygenerowano i zapisano liczby algorytmem ACG (LCG seeded)");
}
