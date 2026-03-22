#pragma once
#include "list.h"

template <typename T> bool List<T>::append(const T &value) {
  ensure_capacity(size_ + 1);
  data_[size_++] = value;
  return true;
}

template <typename T> void List<T>::ensure_capacity(int desired) {
  if (desired <= capacity_) {
    return;
  }

  int new_capacity = capacity_ == 0 ? 4 : capacity_ * 2;
  while (new_capacity < desired) {
    new_capacity *= 2;
  }

  std::unique_ptr<T[]> new_data = std::make_unique<T[]>(new_capacity);
  for (int i = 0; i < size_; ++i) {
    new_data[i] = data_[i];
  }

  data_ = std::move(new_data);
  capacity_ = new_capacity;
}
