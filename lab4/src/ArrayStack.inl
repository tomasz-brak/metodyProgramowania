#pragma once
#include "ArrayStack.h"
#include <stdexcept>

/**
 * @brief Initialize with capacity
 */
template <typename T>
ArrayStack<T>::ArrayStack(int initialCapacity)
    : capacity(initialCapacity > 0 ? initialCapacity : 10), current_size(0) {
  data = new T[capacity];
  this->first = nullptr;
  this->last = nullptr;
}

/**
 * @brief Init with a list of elements
 *
 * @tparam T type of the stack
 * @param initList inital list of elements LIFO
 * @return
 */
template <typename T>
ArrayStack<T>::ArrayStack(std::initializer_list<T> initList)
    : capacity(initList.size() > 0 ? initList.size() * 2 : 10),
      current_size(0) {
  data = new T[capacity];
  this->first = nullptr;
  this->last = nullptr;
  for (const auto &item : initList) {
    this->put(item);
  }
}

/**
 * @brief Cleanup the memory
 *
 * @tparam T Type of the stack element
 */
template <typename T> ArrayStack<T>::~ArrayStack() {
  delete[] data;
  while (this->first) {
    auto temp = this->first;
    this->first = this->first->next;
    delete temp;
  }
}

/**
 * @brief Get the current size of the stack
 *
 * @tparam T Type of the stack element
 */
template <typename T> int ArrayStack<T>::size() { return current_size; }

/**
 * @brief Push an element on the top of the stack, adds it in to the internal
 * array Adds to the end of the array and updates the 'first' pointer to point
 */
template <typename T> void ArrayStack<T>::put(T const &element) {
  if (current_size == capacity) {
    capacity *= 2;
    T *newData = new T[capacity];
    for (int i = 0; i < current_size; ++i)
      newData[i] = data[i];
    delete[] data;
    data = newData;
  }

  data[current_size++] = element;

  auto newNode = new stackItem<T>{this->first, data[current_size - 1]};
  if (!this->last) {
    this->last = newNode;
  }
  this->first = newNode;
}

/**
 * @brief Pop an element from the top
 * Removes from the end of the array and updates @ref first to the next node.
 * @return Value of the removed element
 */
template <typename T> T ArrayStack<T>::pop() {
  if (current_size == 0)
    throw std::out_of_range("Stack is empty");

  T val = data[--current_size];

  auto temp = this->first;
  this->first = this->first->next;
  delete temp;

  if (current_size == 0) {
    this->last = nullptr;
  }

  return val;
}

/**
 * @brief Reference to the top of the stack
 *
 * @tparam T returns a reference to the first element of the stack
 * @return Reference of type @ref T to the element that is on top of the stack
 */
template <typename T> T &ArrayStack<T>::top() {
  if (current_size == 0)
    throw std::out_of_range("Stack is empty");
  return this->first->data;
}
