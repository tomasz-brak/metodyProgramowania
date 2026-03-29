#pragma once
#include "containers/container.h"
#include <functional>
#include <memory>
#include <utility>
template <typename T>
std::pair<int, int>
findRangeBoundaries(const Container<T> &container, int pos,
                    std::function<long long(const T &)> match);

template <typename T>
int interpolationSearch(const Container<T> &container, const T &target,
                        std::function<long long(const T &)> match);

template <typename T>
int binarySearch(const Container<T> &container, const T &target,
                 std::function<long long(const T &)> match);

template <typename T>
concept Hashable = requires(T a) {
  { std::hash<T>{}(a) } -> std::convertible_to<std::size_t>;
};

template <Hashable T>
std::unique_ptr<Container<T>> uniqueElements(const Container<T> &values);

#include "alghoritms.inl"
