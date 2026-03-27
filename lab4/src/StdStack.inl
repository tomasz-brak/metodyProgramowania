#pragma once
#include "StdStack.h"

/**
 * @brief Init an stdStack with a list of elements
 *
 * @tparam T type of elements
 * @param initList list of elements to init with
 */
template <typename T> StdStack<T>::StdStack(std::initializer_list<T> initList) {
  for (auto const &item : initList) {
    put(item);
  }
}

/**
 * @brief Put an element on top of the stack
 *
 * @tparam T Type of elements in the stack
 * @param element Element to put into the stack
 */
template <typename T> void StdStack<T>::put(T const &element) {
  s.push(element);

  // Update formatter list
  auto *newItem = new stackItem<T>{this->first, element};

  this->first = newItem;

  if (this->last == nullptr) {
    this->last = newItem;
  }

  count++;
}

/**
 * @brief Pops the first element from the top of the stack
 *
 * @tparam T Type of the stack elements
 * @return Element that was removed
 */
template <typename T> T StdStack<T>::pop() {
  if (this->isEmpty()) {
    throw std::runtime_error("Stack is empty");
  }

  s.pop();

  // Update linked list for the formatter
  stackItem<T> *oldFirst = this->first;
  T data = oldFirst->data;

  this->first = oldFirst->next;

  if (this->first == nullptr) {
    this->last = nullptr;
  }

  delete oldFirst;
  count--;

  return data;
}

template <typename T> int StdStack<T>::size() { return this->count; }

/**
 * @brief Top element of the stack
 *
 * @tparam T Type of stack elements
 * @return A reference to the top element of the stack
 */
template <typename T> T &StdStack<T>::top() {
  if (this->isEmpty()) {
    throw std::runtime_error("Stack is empty");
  }
  return s.top();
}
