#pragma once
#include "StackBase.h"
#include <initializer_list>

template <typename T> class ArrayStack : public StackBase<T> {
public:
  ArrayStack(int initialCapacity = 10);
  ArrayStack(std::initializer_list<T> initList);
  ~ArrayStack() override;

  int size() override;
  void put(T const &element) override;
  T pop() override;
  T &top() override;

private:
  void syncPointers();

  int capacity;
  int current_size;

  T(*data);
};

template <typename T, typename CharT>
struct std::formatter<ArrayStack<T>, CharT>
    : std::formatter<StackBase<T>, CharT> {};

#include "ArrayStack.inl"
