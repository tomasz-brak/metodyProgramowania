#pragma once
#include <stdexcept>

template <typename T> class DirectionalList {
private:
  struct Node {
    T data;
    Node *next;
    Node(const T &d) : data(d), next(nullptr) {}
  };

  Node *head;
  size_t _size;

public:
  DirectionalList() : head(nullptr), _size(0) {}

  ~DirectionalList() { clear(); }

  /**
   * @brief Create an element and add it to the list
   *
   * @param value
   */
  void push_back(const T &value) {
    Node *newNode = new Node(value);
    if (!head) {
      head = newNode;
    } else {
      Node *temp = head;
      while (temp->next)
        temp = temp->next;
      temp->next = newNode;
    }
    _size++;
  }

  /**
   * @brief Clear the list and delete all elements
   */
  void clear() {
    while (head) {
      Node *temp = head;
      head = head->next;
      delete temp;
    }
    _size = 0;
  }

  size_t size() const { return _size; }

  /**
   * @class Iterator
   * @brief iterator for the list
   * create an iterator for the one way list so it can be used in loops
   */
  struct Iterator {
    Node *current;
    typedef std::forward_iterator_tag iterator_category;
    typedef T value_type;
    typedef std::ptrdiff_t difference_type;
    typedef T *pointer;
    typedef T &reference;

    reference operator*() const { return current->data; }
    Iterator &operator++() {
      current = current->next;
      return *this;
    }
    bool operator!=(const Iterator &other) const {
      return current != other.current;
    }
    bool operator==(const Iterator &other) const {
      return current == other.current;
    }
  };

  Iterator begin() const { return Iterator{head}; }
  Iterator end() const { return Iterator{nullptr}; }

  /**
   * @brief Iterate from the head of the list and find the element with the
   * provied index
   *
   * @param index Index of the element
   * @return
   */
  T &at(size_t index) {
    Node *temp = head;
    for (size_t i = 0; i < index; ++i) {
      if (!temp)
        throw std::out_of_range("Index out of bounds");
      temp = temp->next;
    }
    if (!temp)
      throw std::out_of_range("Index out of bounds");
    return temp->data;
  }
};
