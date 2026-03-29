#pragma once
#include "OneWayStack.h"
#include "logging.h"
#include <format>
#include <initializer_list>
#include <stdexcept>

/**
 * @brief Initialize with the list of elements
 *
 * @tparam T type of the element
 * @param initList list of elements to init with
 */
template <typename T>
OneWayStack<T>::OneWayStack(std::initializer_list<T> initList) {
  for (const auto &item : initList) {
    put(item);
  }
  Logger::debug("Finished inserting... last is ");
}

/**
 * @brief Put the element in to the stack
 *
 * Puts the element in to the stack
 * @tparam T Type of the stack elements
 * @param element element to put in to the stack
 */
template <typename T> void OneWayStack<T>::put(T const &element) {
  stackItem<T> *newNode = new stackItem<T>{this->first, element};
  this->first = newNode;
  this->count++;
}

/**
 * @brief Get the top element and remove
 *
 * @tparam T type of stack elements
 * @return element of the stack that was removed
 */
template <typename T> T OneWayStack<T>::pop() {
  if (this->first == nullptr) {
    Logger::critical("Tryied to pop an element out of an empty stack");
    throw std::out_of_range("Stack is empty");
  }

  stackItem<T> *oldFirst = this->first;
  T value = oldFirst->data;

  this->first = oldFirst->next;
  this->count--;
  delete oldFirst;

  return value;
}
/**
 * @brief Get the reference to the top element
 *
 * @tparam T Type of stack elements
 * @return Reference of type @ref T to the top element of the stack
 */
template <typename T> T &OneWayStack<T>::top() { return this->first->data; }
