#include "Files.h"
#include "logging.h"
#include "src/ArrayStack.h"
#include "src/OneWayStack.h"
#include "src/StackBase.h"
#include "src/StdStack.h"
#include <cerrno>
#include <concepts>
#include <fstream>

template <typename T, typename type>
concept AnyStack = std::derived_from<T, StackBase<type>>;

template <AnyStack<int> StackTypeToTest> void testStack() {
  int i1 = 1;
  int i2 = 2;
  int i3 = 3;
  auto stack = StackTypeToTest({i1, i2, i3});

  Logger::debug("Testing stack of type: {}", typeid(StackTypeToTest).name());
  Logger::debug("Stack is: {}", stack);
  Logger::debug("Stack is empty: {}", stack.isEmpty());
  Logger::debug("Number of elements: {}", stack.size());
  Logger::debug("Top element is: {}", stack.top());
  stack.put(4);
  Logger::debug("Placed a new element, stack is: {}", stack);
  Logger::debug("Poped last element with value: {}", stack.pop());
  Logger::debug("Poped last element with value: {}", stack.pop());
  Logger::debug("Stack is: {}", stack);
}

int main() {
  testStack<StdStack<int>>();
  testStack<OneWayStack<int>>();
  testStack<ArrayStack<int>>();
  std::ifstream file = promptOpenFile<std::ifstream>(
      "podaj nazwe pliku wejsciowego(testowego): ",
      std::function<bool(std::string)> verifier) return 0;
}
