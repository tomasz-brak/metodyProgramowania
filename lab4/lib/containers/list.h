#pragma once

#include "container.h"

#include <memory>
#include <set>

template <typename T> struct is_unique_ptr : std::false_type {};
template <typename T, typename D>
struct is_unique_ptr<std::unique_ptr<T, D>> : std::true_type {};

template <typename T> class List : public Container<T> {
public:
  List() = default;
  List(std::initializer_list<T> init);

  template <typename Compare> List(std::set<T, Compare> &&s);
  List(const std::set<T> &s);
  ~List() override = default;

  T &operator[](int i) override { return data_[i]; }
  const T &operator[](int i) const override { return data_[i]; }

  T &at(int i) override { return data_[i]; }

  const T &at(int i) const override { return data_[i]; }

  T *begin() override { return data_.get(); }
  T *end() override { return data_.get() + size_; }
  const T *begin() const override { return data_.get(); }
  const T *end() const override { return data_.get() + size_; }

  int size() const override { return size_; }

  void ensure_capacity(int desired);

  void slice(int min, int max);

  bool append(T value) override;
  std::unique_ptr<Container<T>> copy() const override;

  std::unique_ptr<List<T>> clone() const;

private:
  std::unique_ptr<T[]> data_;
  int size_ = 0;
  int capacity_ = 0;
};
template <typename T>
using UniqueList = std::unique_ptr<List<std::unique_ptr<T>>>;
#include "list.inl"
