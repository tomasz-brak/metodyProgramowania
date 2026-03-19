#pragma once

#include <functional>
#include <string_view>

constexpr auto SEPARATORS = " \t\f\v";

template <typename T> class Container {
public:
  virtual ~Container() = default;

  virtual T &operator[](int i) = 0;
  virtual const T &operator[](int i) const = 0;

  virtual T *begin() = 0;
  virtual T *end() = 0;

  virtual int size() const = 0;

  virtual bool append(const T &value) = 0;

  void load(std::string_view &line,
            std::function<T *(const std::string_view &)> converter);

private:
  void ensure_capacity(int desired);
};
