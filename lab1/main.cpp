#include <cctype>
#include <cmath>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <stdexcept>
#include <string>

struct Table {
  int *data;
  int size;
};

void take_input(Table data) {}

Table create_table(int len) {
  Table T;
  T.data = new int[len];
  T.size = len;
  return T;
}

std::string take_input(std::function<bool(std::string)> validator) {
  std::string anwser;

  while (true) {
    std::cin >> anwser;
    if (validator(anwser)) {
      break;
    }
    std::cout << "Wrong input! Try again:\n";
  }
  return anwser;
}

bool isNumber(std::string s) {
  for (char ch : s) {
    if (!std::isdigit(ch)) {
      return false;
    }
  }
  return true;
}

bool isValidTableSize(std::string s) {
  return isNumber(s) && std::stoi(s) >= 1 && std::stoi(s) <= std::pow(10, 6);
}

bool isValidTableElement(std::string s) {
  try {
    return abs(std::stoi(s)) < 2000;
  } catch (std::invalid_argument) {
    return false;
  }
}

Table input_table(int size) {
  Table table = create_table(size);
  for (int i = 0; i < size; i++) {
    auto anwser = take_input(isValidTableElement);
    table.data[i] = std::stoi(anwser);
  }
  return table;
}

void work(Table table) {
  for (int i = 0; i < table.size; i++) {
    table[]
  }
}

int main() {
  Table table;
  table = input_table(std::stoi(take_input(isValidTableSize)));
  return 0;
}
