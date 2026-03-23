#pragma once
#include "alghoritms.h"
#include "containers/container.h"
#include "containers/list.h"
#include <functional>
#include <memory>
#include <set>
template <typename T>
std::pair<int, int>
findRangeBoundaries(const Container<T> &container, int pos,
                    std::function<long long(const T &)> match) {
  if (pos < 0 || pos >= static_cast<int>(container.size())) {
    return {-1, -1};
  }

  int first = pos;
  int last = pos;
  auto targetVal = match(container.at(pos));

  // Find first occurrence (scan left)
  while (first > 0 && match(container.at(first - 1)) == targetVal) {
    first--;
  }

  // Find last occurrence (scan right)
  while (last < static_cast<int>(container.size()) - 1 &&
         match(container.at(last + 1)) == targetVal) {
    last++;
  }

  return {first, last};
}

template <typename T>
int interpolationSearch(const Container<T> &container, const T &target,
                        std::function<long long(const T &)> match) {
  int low = 0;
  int high = static_cast<int>(container.size()) - 1;
  long long targetVal = match(target);

  while (low <= high) {
    long long lowVal = match(container.at(low));
    long long highVal = match(container.at(high));

    if (targetVal < lowVal || targetVal > highVal)
      break;

    int pos;
    if (highVal == lowVal) {
      pos = low;
    } else {
      pos = low + static_cast<int>((double(high - low) / (highVal - lowVal)) *
                                   (targetVal - lowVal));
    }
    long long posVal = match(container.at(pos));

    if (posVal == targetVal) {
      return pos;
    }

    if (posVal < targetVal) {
      low = pos + 1;
    } else {
      high = pos - 1;
    }
  }
  return -1;
}

template <typename T>
int binarySearch(const Container<T> &container, const T &target,
                 std::function<long long(const T &)> match) {
  int low = 0;
  int high = container.size() - 1;
  auto targetValue = match(target);

  while (low <= high) {
    int pos = low + (high - low) / 2;
    auto posValue = match(container.at(pos));

    if (posValue == targetValue) {
      return pos;
    }
    if (posValue > targetValue) {
      high = pos - 1;
    } else {
      low = pos + 1;
    }
  }
  return -1;
}

template <Hashable T>
std::unique_ptr<Container<T>> uniqueElements(Container<T> &values) {
  auto comp = [](const T &a, const T &b) { return *a < *b; };
  // https://stackoverflow.com/questions/70656353/declare-and-define-custom-stdset-comparator
  auto hashset = std::set<T, decltype(comp)>(comp);

  for (auto &value : values) {
    hashset.insert(std::move(value));
  }

  return std::make_unique<List<T>>(std::move(hashset));
};
