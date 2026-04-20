#pragma once
#include "containers/container.h"
#include "list.h"
#include "logging.h"
#include <algorithm>
#include <set>

/**
 * @brief Appends a value to the storage
 * capacity is managed automaticaly. Value gets moved
 * @tparam T
 * @param value
 * @return
 */
template <typename T> bool List<T>::append(T value) {
  ensure_capacity(size_ + 1);
  data_[size_] = std::move(value);
  size_++;
  return true;
}

/**
 * @brief Extends the capacity of the List storage
 * Extends the capacity of the underlaying storage pointer, doubling every time.
 * @tparam T
 * @param desired
 */
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

/**
 * @brief initializer list constructor
 * Creates a list object form an initializer list of elements
 * @tparam T Container type
 * @param init initializer list
 */
template <typename T> List<T>::List(std::initializer_list<T> init) {
  ensure_capacity(init.size());
  for (const auto &val : init) {
    append(val);
  }
}

/**
 * @brief Creates a list from a set
 *
 * @tparam T Container type
 * @param s Set to be created from (copy values)
 */
template <typename T> List<T>::List(const std::set<T> &s) {
  ensure_capacity(s.size());
  for (const auto &val : s) {
    append(val);
  }
}

/**
 * @brief Creates a List from a Set of unique_ptr
 *
 * @tparam Compare Any comparator is ok
 * @param s The set with unique_ptr objects
 */

template <typename T>
template <typename Compare>
List<T>::List(std::set<T, Compare> &&s) {
  ensure_capacity(s.size());
  for (auto &val : s) {
    append(std::move(const_cast<T &>(val)));
  }
}

/**
 * @brief Copies the List
 *  - For lists consisting of non-pointer types it perfroms a normal copy
 *  - Pointers can provide a #copy function
 *  - If not copy constructor is used
 * @tparam T Container type
 * @return unique_ptr with the copy of the container
 */
template <typename T> std::unique_ptr<Container<T>> List<T>::copy() const {
  auto newList = std::make_unique<List<T>>();
  newList->ensure_capacity(this->size_);

  for (int i = 0; i < this->size_; ++i) {
    if constexpr (is_unique_ptr<T>::value || std::is_pointer_v<T>) {
      if (!data_[i]) {
        newList->append(nullptr);
        continue;
      }
    }

    if constexpr (is_unique_ptr<T>::value) {
      using ElementType = typename T::element_type;

      // Case A: Element has a .copy() method (like a nested List)
      if constexpr (requires(const ElementType &e) { e.copy(); }) {
        newList->append(data_[i]->copy());
      }
      // Case B: Element is copy-constructible
      else if constexpr (std::is_copy_constructible_v<ElementType>) {
        newList->append(std::make_unique<ElementType>(*data_[i]));
      } else {
        static_assert(std::is_copy_constructible_v<ElementType>,
                      "Type inside unique_ptr must have a .copy() method or a "
                      "copy constructor.");
      }
    }

    else {
      newList->append(data_[i]);
    }
  }
  return newList;
}

/**
 * @brief Slices the list in place
 *
 * @tparam T Container type
 * @param min low bound of the slice (index)
 * @param max high bound of the slice (index)
 */
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

template <typename T> std::unique_ptr<List<T>> List<T>::clone() const {
  return std::unique_ptr<List<T>>(
      static_cast<List<T> *>(this->copy().release()));
}

template <typename T, typename CharT>
struct std::formatter<List<T>, CharT> : std::formatter<Container<T>, CharT> {};
