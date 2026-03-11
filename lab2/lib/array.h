#pragma once
template <typename T, int s> class Array {
public:
  T data[s];
  int size = s;

  T &operator[](int i) { return data[i]; }

  const T &operator[](int i) const { return data[i]; }

  T *begin() { return data; }

  T *end() { return data + size; }
};
