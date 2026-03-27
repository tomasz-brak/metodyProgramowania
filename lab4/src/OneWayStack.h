#pragma once
#include "StackBase.h"
#include <initializer_list>

template <typename T> class OneWayStack : public StackBase<T> {
public:
  ~OneWayStack() = default;
  OneWayStack(std::initializer_list<T> initList);

  T *begin() { return &(this->first->data); }
  T *end() { return &((this->last + 1)->data); }

  const T *begin() const { return &(this->first->data); }
  const T *end() const { return &((this->last)->data); }

  bool isEmpty() { return begin() == end() ? 1 : 0; }
  int size() override { return count; }

  void put(T const &element) override;

  T &top() override;

  T pop() override;

private:
  int count = 0;
};
template <typename T, typename CharT>
struct std::formatter<OneWayStack<T>, CharT>
    : std::formatter<StackBase<T>, CharT> {};

#include "OneWayStack.inl"
