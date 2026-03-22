#pragma once

#include "container.h"

#include <memory>

template <typename T> class List : public Container<T> {
public:
  List() = default;
  ~List() override = default;

  T &operator[](int i) override { return data_[i]; }
  const T &operator[](int i) const override { return data_[i]; }

  T *begin() override { return data_.get(); }
  T *end() override { return data_.get() + size_; }

  int size() const override { return size_; }

  void ensure_capacity(int desired);

  bool append(const T &value) override;

private:
  std::unique_ptr<T[]> data_;
  int size_ = 0;
  int capacity_ = 0;
};

#include "list.inl"
