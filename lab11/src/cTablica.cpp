#include "cTablica.h"
#include <deque>
#include <print>

/**
 * @brief Sorts the internal collection using the Bubble sort alghorithm
 *
 * Structure:
 * While the elements are not sorted iterates through the array, swapping
 * elements if one is found to be higher then other.
 * If no swaps occured we know the collection is in order.
 *
 * @param test Test result object
 */
void cTablica::sortbobel(testResult &test) {
  if (data.size() < 2) {
    return;
  }
  bool isSorted = false;
  while (!isSorted) {
    isSorted = true;
    for (auto it = data.begin(); it < data.end() - 1; it++) {
      test.comparisions++;
      if (*it > *(it + 1)) {
        isSorted = false;
        std::swap(*it, *(it + 1));
        test.swaps++;
      }
    }
  }
}

/**
 * @brief Partitions a subarray around a pivot using the Lomuto's scheme.
 *
 * Last element of the range is selected as a pivot.
 * We try to arrage the array in a way that the pivot sits in the middle,
 * splitting lowest largest values.
 *
 * @details
 * Algorithms chooses a pivot candidate (last element). Then iterates through
 * the subarray. Declare 2 pointers fast (j ++ every execution of the loop) slow
 * (i before i all elements are <= pivot; i=low-1)
 *
 * Iterate through the subarray, if an element j is lower then the pivot,
 * i+1-th(being the element after i means its larger) and j-th element get
 * swaped.
 *
 * After swapping through all of the array we place the pivot in the i+1
 * location
 *
 * @param low Lower bound
 * @param high Higer bound
 * @param tab table reference
 * @param test test result agragator
 * @return location of the pivot
 */
int partitionLomuto(size_t low, size_t high, cTablica &tab, testResult &test) {
  int pivot = tab.data[high];
  int i = low - 1;

  for (size_t j = low; j < high; ++j) {
    test.comparisions++;
    if (tab.data[j] <= pivot) {
      i++;
      std::swap(tab.data[j], tab.data[i]);
      test.swaps++;
    }
  }
  std::swap(tab.data[i + 1], tab.data[high]);
  test.swaps++;
  return i + 1;
}

/**
 * @brief Sorts the array using QuickSort(Lomuto as the partition function)
 *
 * By continuosly partitioning the array around pivots arranges smaller and
 * smaller arrays in order
 *
 * If segments cointain more then one element the are added to the queue to
 * process again.
 *
 * @param test test result agragator
 */
void cTablica::sortlomuto(testResult &test) {
  if (this->data.size() < 2) {
    return;
  }
  int low = 0;
  int high = this->data.size() - 1;

  // holds left and right bounds
  std::deque<std::pair<int, int>> q;
  q.push_back(std::make_pair(low, high));
  while (q.size() > 0) {
    auto [low, high] = q.back();
    q.pop_back();
    auto index = partitionLomuto(low, high, *this, test);
    test.comparisions++;
    if (index - 1 > low) {
      q.push_back(std::make_pair(low, index - 1));
    }
    test.comparisions++;
    if (index + 1 < high) {
      q.push_back(std::make_pair(index + 1, high));
    }
  }
}

/**
 * @brief Sorts the internal collection using QuickSort(with hoare's comparision
 * function)
 *
 * Select a pivot = half of the array and init pointers to front and back of the
 * array. Move i and j unitil they find repectivly: element >= pivot and element
 * <= pivot. Swap the values.
 *
 * Repeat until i and j cross each other
 *
 * Partition index is j is returned.
 *
 * Use a stack to itrativly QuickSort.
 *
 * @param test test result agragator
 */
void cTablica::sorthoare(testResult &test) {
  std::vector<std::pair<int, int>> stos;

  stos.push_back({0, data.size() - 1});

  while (!stos.empty()) {
    int low = stos.back().first;
    int high = stos.back().second;
    stos.pop_back();
    test.comparisions++;
    if (low < high) {
      int pivot = data[low + (high - low) / 2]; // elemenmt srodka przedzialu
      int i = low - 1;
      int j = high + 1;

      while (true) {
        do {
          // szuka po lewej stronie elementu wiekszego lub rownego pivot
          i++;
          test.comparisions++;
        } while (data[i] < pivot);

        do {
          // szuka po prawej stronie elementu mniejszego lub rownego pivot
          j--;
          test.comparisions++;
        } while (data[j] > pivot);
        // jesli i oraz j sie minely, przerywa petle
        test.comparisions++;
        if (i >= j) {
          break;
        }
        // jak sie nie minely to zamieniamy i oraz j miejscami
        std::swap(data[i], data[j]);
        test.swaps++;
      }
      int wynik = j;

      // odswiezanie stosu
      stos.push_back({low, wynik});
      stos.push_back({wynik + 1, high});
    }
  }
}

struct children {
  int *left;
  int *right;
};

/**
 * @brief Sorts the underlying collection using Heap sort
 *
 * Creates a heap inside the data vector.
 * Heap loop arrages items in the vector to follow heap rule (parent is always >
 * then child).
 * @details
 * Process each parent of an element in a tree bottom up (i = n/2 -1)
 * max(parent, LChild, RChild) becomes the parent (whichever values was chosen
 * gets replaced by the parent too[swap])
 *
 * *Heap sort*
 * Largest element is first move it to the back of the vector
 * Find largest among the children and move them to the front
 * repeat until all elements are moved to the sorted area
 *
 * @param test test result agragator
 */
void cTablica::sortkopcowanie(testResult &test) {
  if (this->data.size() < 2) {
    return;
  }

  int n = this->data.size();

  for (int i = (n / 2) - 1; i >= 0; --i) {
    int current = i;

    while (true) {
      int left = 2 * current + 1;
      int right = 2 * current + 2;
      int largest = current;

      test.comparisions++;
      if (left < n) {
        test.comparisions++;
        if (this->data[left] > this->data[largest]) {
          largest = left;
        }
      }

      test.comparisions++;
      if (right < n) {
        test.comparisions++;
        if (this->data[right] > this->data[largest]) {
          largest = right;
        }
      }

      test.comparisions++;
      if (largest != current) {
        std::swap(this->data[current], this->data[largest]);
        test.swaps++;
        current = largest;
      } else {
        break;
      }
    }
  }
  int heapSize = this->data.size();

  while (heapSize > 1) {
    std::swap(this->data[0], this->data[--heapSize]);
    test.swaps++;

    int curr = 0;
    while (true) {
      int left = 2 * curr + 1;
      int right = 2 * curr + 2;
      int largest = curr;

      if (left < heapSize && data[left] > data[largest])
        largest = left;
      if (right < heapSize && data[right] > data[largest])
        largest = right;

      test.comparisions += 2;

      if (largest != curr) {
        std::swap(this->data[curr], this->data[largest]);
        test.swaps++;
        curr = largest;
      } else {
        break;
      }
    }
  }
}

/**
 * @brief Sort all elements using bi-directional bubble sort
 * Sort using normal bubble logic swapping if next > current, then reverse after
 * hitting the end if next < current swap.
 * @param test test result agragator
 */
void cTablica::sortwach(testResult &test) {
  if (data.size() < 2)
    return;

  bool swapped = true;
  int start = 0;
  int end = data.size() - 1;

  while (swapped) {
    swapped = false;

    // przejscie w prawo
    for (int i = start; i < end; i++) {
      test.comparisions++;
      if (data[i] > data[i + 1]) {
        std::swap(data[i], data[i + 1]);
        test.swaps++;
        swapped = true;
      }
    }
    test.comparisions++;
    if (!swapped) {
      break;
    } // jesli tak to juz jest posortowana

    swapped = false;
    end--; // ostatnni element juz jest na swoim miejscu
    // przejscie w lewo
    for (int i = end - 1; i >= start; i--) {
      test.comparisions++;
      if (data[i] > data[i + 1]) {
        std::swap(data[i], data[i + 1]);
        test.swaps++;
        swapped = true;
      }
    }
    start++;
  }
}
/**
 * @brief Generic sorter test function
 *
 * @param sorter Function member of the \t cTablica class that sorts in buildin
 * vector of data.
 * @param name Name of the sort function
 */
void cTablica::test(std::function<void(cTablica *, testResult &)> sorter,
                    const std::string &name) {
  std::println("Sorting with {}", name);
  // std::println("{} from:\n {}", name, this->data);
  auto test = testResult{};
  auto start = std::chrono::high_resolution_clock::now();
  sorter(this, test);
  auto end = std::chrono::high_resolution_clock::now();
  // std::println("to: {}", this->data);
  std::chrono::duration<double, std::milli> duration = end - start;
  std::println("comparisions done: {:05}, swaps done: {:05}, finished in: {}ms",
               test.comparisions, test.swaps, duration.count());
  std::println("------------------------------------------");
}
