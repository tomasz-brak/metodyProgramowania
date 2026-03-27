#pragma once
#include "StackBase.h"
#include <initializer_list>
#include <stack>

template <typename T> class StdStack : public StackBase<T> {
public:
  StdStack() = default;
  StdStack(std::initializer_list<T> initList);
  ~StdStack() override = default;

  int size() override;
  T &top() override;
  void put(T const &element) override;
  T pop() override;

private:
  std::stack<T> s{};
  int count = 0;
};

template <typename T, typename CharT>
struct std::formatter<StdStack<T>, CharT>
    : std::formatter<StackBase<T>, CharT> {};

#include "StdStack.inl"
