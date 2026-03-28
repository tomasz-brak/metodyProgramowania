#include "Files.h"
#include "logging.h"
#include "src/ArrayStack.h"
#include "src/OneWayStack.h"
#include "src/StackBase.h"
#include "src/StdStack.h"
#include <concepts>
#include <fstream>

template <typename T, typename type>
concept AnyStack = std::derived_from<T, StackBase<type>>;

template <AnyStack<int> StackTypeToTest>
void testStack(std::ifstream &testFile) {
  Logger::debug("Testing stack of type: {}", typeid(StackTypeToTest).name());
  auto stack = StackTypeToTest({});
  Logger::debug("Initialized the stack");
  stack.load(testFile);
  Logger::debug("Loaded from a file");

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

  std::ifstream file = promptOpenFile<std::ifstream>(
      "podaj nazwe pliku wejsciowego(testowego): ",
      [](std::string _) { return true; });

  testStack<StdStack<int>>(file);
  file.clear();
  file.seekg(0, std::ios::beg);
  testStack<OneWayStack<int>>(file);
  file.clear();
  file.seekg(0, std::ios::beg);
  testStack<ArrayStack<int>>(file);
  file.clear();
  file.seekg(0, std::ios::beg);
  return 0;
}
