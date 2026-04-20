#pragma once
#include <functional>
#include <span>
template <typename T>
int bs_recursive(std::span<const T> data, std::function<int(T)> enumerator,
                 bool find_last = false) {
  if (data.empty()) {
    return -1;
  }

  size_t mid = data.size() / 2;
  int eVal = enumerator(data[mid]);

  if (eVal == 0) {
    if (find_last) {
      int rightRes = bs_recursive(data.subspan(mid + 1), enumerator, true);
      if (rightRes != -1) {
        return (int)(mid + 1 + rightRes);
      }
      return (int)mid;
    } else {
      int leftRes = bs_recursive(data.first(mid), enumerator, false);
      if (leftRes != -1) {
        return leftRes;
      }
      return (int)mid;
    }
  }

  if (eVal < 0) {
    int res = bs_recursive(data.subspan(mid + 1), enumerator, find_last);
    return (res == -1) ? -1 : (int)(mid + 1 + res);
  } else {
    return bs_recursive(data.first(mid), enumerator, find_last);
  }
}

template <typename T>
int bs_iterative(std::span<const T> data, std::function<int(T)> enumerator,
                 bool find_last = false) {
  int left = 0;
  int right = (int)data.size() - 1;
  int result = -1;

  while (left <= right) {
    int mid = left + (right - left) / 2;
    int eVal = enumerator(data[mid]);

    if (eVal == 0) {
      result = mid;
      if (find_last) {
        left = mid + 1;
      } else {
        right = mid - 1;
      }
    } else if (eVal < 0) {
      left = mid + 1;
    } else {
      right = mid - 1;
    }
  }

  return result;
}
