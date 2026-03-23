#pragma once
#include "containers/container.h"
#include "list.h"
#include "logging.h"
#include <algorithm>
#include <set>

template <typename T> bool List<T>::append(T value) {
  ensure_capacity(size_ + 1);
  data_[size_] = std::move(value);
  size_++;
  return true;
}

template <typename T> void List<T>::ensure_capacity(int desired) {
  if (this == nullptr) {
    Logger::critical("Attempted to extend a null list!");
  }
  if (desired <= capacity_) {
    return;
  }

  int new_capacity = (capacity_ == 0) ? 4 : capacity_;
  while (new_capacity < desired) {
    new_capacity *= 2;
  }

  std::unique_ptr<T[]> new_data = std::make_unique<T[]>(new_capacity);

  // Only attempt to move if we actually have existing data
  if (data_) {
    for (int i = 0; i < size_; ++i) {
      new_data[i] = std::move(data_[i]);
    }
  }

  data_ = std::move(new_data);
  capacity_ = new_capacity;
}

template <typename T> List<T>::List(std::initializer_list<T> init) {
  ensure_capacity(init.size());
  for (const auto &val : init) {
    append(val);
  }
}

template <typename T> List<T>::List(const std::set<T> &s) {
  ensure_capacity(s.size());
  for (const auto &val : s) {
    append(val);
  }
}

template <typename T>
template <typename Compare>
List<T>::List(std::set<T, Compare> &&s) {
  ensure_capacity(s.size());
  for (auto &val : s) {
    append(std::move(const_cast<T &>(val)));
  }
}

template <typename T> std::unique_ptr<Container<T>> List<T>::copy() const {
  auto newList = std::make_unique<List<T>>();
  newList->ensure_capacity(this->size_);

  for (int i = 0; i < this->size_; ++i) {
    if (!data_[i]) {
      newList->append(nullptr);
      continue;
    }

    if constexpr (is_unique_ptr<T>::value) {
      using ElementType = typename T::element_type;

      // Polymorphic Clone
      if constexpr (requires(const ElementType &e) { e.copy(); }) {
        newList->append(data_[i]->copy());
      }
      // Try to use copy constructor if it exists
      else if constexpr (std::is_copy_constructible_v<ElementType>) {
        newList->append(std::make_unique<ElementType>(*data_[i]));
      } else {
        // Error out if not able to copy
        static_assert(std::is_copy_constructible_v<ElementType>,
                      "Type inside unique_ptr must have a .copy() method or a "
                      "copy constructor.");
      }
    } else {
      // Standard non-pointer types
      newList->append(data_[i]);
    }
  }
  return newList;
}

template <typename T> void List<T>::slice(int min, int max) {
  // Bounds checking and validation
  if (min < 0)
    min = 0;
  if (max > size_)
    max = size_;
  if (min >= max || min >= size_) {
    // Resulting slice is empty
    data_.reset();
    size_ = 0;
    capacity_ = 0;
    return;
  }

  int new_size = max - min;
  int new_capacity = new_size;
  std::unique_ptr<T[]> new_data = std::make_unique<T[]>(new_capacity);

  // Move elements from the slice range to the new storage
  for (int i = 0; i < new_size; ++i) {
    new_data[i] = std::move(data_[min + i]);
  }

  // Update list state
  data_ = std::move(new_data);
  size_ = new_size;
  capacity_ = new_capacity;
}
