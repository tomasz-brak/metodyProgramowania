
#include "sets.h"
#include "containers/list.h"
#include "logging.h"
#include <memory>

/**
 * @brief Generates all subsets of an n-element set using Gray code.
 *
 * Each subset is represented as a binary sequence of length n.
 * Only one bit changes between consecutive subsets.
 *
 * @param n Size of the set.
 * @return UniqueList containing lists of integers. Each inner list is a binary
 * vector
 */
UniqueList<List<int>> subsetsGray(int n) {
  auto allSubsets = std::make_unique<List<std::unique_ptr<List<int>>>>();
  int generatedAmount = 0;
  int changePoint;

  List<int> currentSubset;
  for (int k = 0; k < n; ++k)
    currentSubset.append(0);

  do {
    allSubsets->append(currentSubset.clone());

    generatedAmount++;
    changePoint = 0;

    int secondPoint = generatedAmount;

    while (secondPoint % 2 == 0) {
      secondPoint /= 2;
      changePoint++;
    }

    if (changePoint < n) {
      currentSubset[changePoint] = 1 - currentSubset[changePoint];
    }
  } while (changePoint < n);

  return allSubsets;
}
/**
 * @brief Generates all k-element subsets of an n-element set in lexicographical
 * order.
 *
 * Generates combinations without repetition.
 * The elements in each subset are sorted in increasing order.
 *
 * @param n Total number of elements in the main set.
 * @param k Number of elements in each subset.
 * @return UniqueList containing lists of integers. Each inner list is a
 * k-element combination.
 */
UniqueList<List<int>> subsetsLex(int n, int k) {
  auto allSubsets = std::make_unique<List<std::unique_ptr<List<int>>>>();

  if (k > n || k < 0)
    return allSubsets;

  List<int> currentSubset;
  for (int i = 1; i <= k; ++i) {
    currentSubset.append(i);
  }

  int p = k;
  while (p >= 1) {
    allSubsets->append(currentSubset.clone());

    if (currentSubset[k - 1] == n) {
      p--;
    } else {
      p = k;
    }

    if (p >= 1) {
      int valAtP = currentSubset[p - 1];
      for (int i = k; i >= p; --i) {
        currentSubset[i - 1] = valAtP + i - p + 1;
      }
    }
  }

  return allSubsets;
}

/**
 * @brief Generates all integer partitions of n.
 *
 * An integer partition is a way of writing n as a sum of positive integers.
 * Generates partitions in descending order.
 *
 * @param n The integer to partition. Must be greater than 0.
 * @return UniqueList containing lists of integers. Each inner list is a valid
 * partition of n.
 */
UniqueList<List<int>> generatePartitions(int n) {
  auto allPartitions = std::make_unique<List<std::unique_ptr<List<int>>>>();

  if (n <= 0) {
    Logger::critical("generatePartitions called with invalid n: {}", n);
    return allPartitions;
  }

  Logger::debug("partition generation for n: {}", n);

  /**
   * partValues: stores the numeric value of the components
   * partCounts: stores the frequency of that value
   * We initialize these with n+1 slots to handle the worst-case
   */
  List<int> partValues;
  List<int> partCounts;

  for (int i = 0; i <= n; ++i) {
    partValues.append(0);
    partCounts.append(0);
  }

  // Initial state: One part with value n
  partValues[0] = n;
  partCounts[0] = 1;
  int lastDistinctIndex = 0;

  while (true) {
    List<int> currentPartition;
    for (int i = 0; i <= lastDistinctIndex; ++i) {
      for (int j = 0; j < partCounts[i]; ++j) {
        currentPartition.append(partValues[i]);
      }
    }

    allPartitions->append(currentPartition.clone());
    Logger::debug("Adding partition: {}", currentPartition);
    if (partValues[0] == 1) {
      Logger::debug("Reached the end partition STOP...");
      break;
    }

    int gatheredSum = 0;

    // Collect trailing 1s
    if (partValues[lastDistinctIndex] == 1) {
      gatheredSum += partCounts[lastDistinctIndex];
      Logger::debug("removed {} trailing 1. current sum: {}",
                    partCounts[lastDistinctIndex], gatheredSum);
      lastDistinctIndex--;
    }
    gatheredSum += partValues[lastDistinctIndex];
    partCounts[lastDistinctIndex]--;

    int nextValueToFill = partValues[lastDistinctIndex] - 1;
    Logger::debug("Decreased part {} to {}. sum to redistribute: {}",
                  partValues[lastDistinctIndex], nextValueToFill, gatheredSum);

    if (partCounts[lastDistinctIndex] > 0) {
      lastDistinctIndex++;
    }

    partValues[lastDistinctIndex] = nextValueToFill;
    partCounts[lastDistinctIndex] = gatheredSum / nextValueToFill;

    Logger::debug("redistributing: {} parts of value {}",
                  partCounts[lastDistinctIndex], nextValueToFill);

    int remainder = gatheredSum % nextValueToFill;
    if (remainder != 0) {
      lastDistinctIndex++;
      partValues[lastDistinctIndex] = remainder;
      partCounts[lastDistinctIndex] = 1;
      Logger::debug("Added remainder part: {}", remainder);
    }
  }

  return allPartitions;
}
