#pragma once

#include "container.h"
#include <string>

template <typename T, int s> class Array : public Container<T> {
public:
  T data[s];
  int size_ = s;

  T &operator[](int i) override { return data[i]; }
  const T &operator[](int i) const override { return data[i]; }

  T *begin() override { return data; }
  T *end() override { return data + size_; }

  void load(std::string &line,
            std::function<T *(const std::string &)> converter);
  int size() const override { return size_; }
};
