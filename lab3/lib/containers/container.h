#pragma once

#include <functional>
#include <memory>
#include <string_view>

constexpr auto SEPARATORS = " \t\f\v";

template <typename T> class Container {
public:
  virtual ~Container() = default;

  virtual T &operator[](int i) = 0;
  virtual const T &operator[](int i) const = 0;

  virtual T &at(int i) = 0;

  virtual const T &at(int i) const = 0;

  virtual T *begin() = 0;
  virtual T *end() = 0;
  virtual const T *begin() const = 0;
  virtual const T *end() const = 0;

  virtual int size() const = 0;

  virtual bool append(T value) = 0;

  virtual std::unique_ptr<Container<T>> copy() const = 0;

  void
  load(const std::string_view &line,
       std::move_only_function<T(const std::string_view &) const> converter);
};

#include "container.inl"
