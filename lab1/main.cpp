#include <cctype>
#include <cmath>
#include <cstdio>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <memory>
#include <string>

struct Table {
  std::unique_ptr<int[]> data;
  int size;
};

struct Anwser {
  int start;
  int end;
  int sum;
};

Table create_table(int len) {
  Table T;
  T.data = std::make_unique<int[]>(len);
  T.size = len;
  return T;
}

std::string take_input(std::function<bool(std::string)> validator) {
  std::string anwser;

  while (true) {
    if (!(std::cin >> anwser))
      break;
    if (validator(anwser)) {
      break;
    }
    std::cout << "Wrong input! Try again:\n";
  }
  return anwser;
}

bool isNumber(std::string s) {
  if (s.empty())
    return false;
  for (char ch : s) {
    if (!std::isdigit(ch)) {
      return false;
    }
  }
  return true;
}

bool isValidTableSize(std::string s) {
  if (!isNumber(s))
    return false;
  long long val = std::stoll(s);
  return val >= 1 && val <= std::pow(10, 6);
}

bool isValidTableElement(std::string s) {
  try {
    size_t pos;
    int val = std::stoi(s, &pos);
    return pos == s.length() && std::abs(val) < 2000;
  } catch (...) {
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

int weight(int a) {
  if (a < 0) {
    return a * 2;
  }
  return a * 3;
}

Anwser kadane(const Table &table) {
  int best_sum = 0;
  int current_sum = 0;

  int best_start = -1;
  int best_end = -1;
  int current_start = 0;

  for (int i = 0; i < table.size; ++i) {
    int w = weight(table.data[i]);
    current_sum += w;

    if (current_sum < 0) {
      current_sum = 0;
      current_start = i + 1;
    } else if (current_sum > best_sum) {
      best_sum = current_sum;
      best_start = current_start;
      best_end = i;
    }
  }

  if (best_sum == 0) {
    return {-1, -1, 0};
  }

  return {best_start, best_end, best_sum};
}

int main() {
  std::string z_input = take_input(isValidTableSize);
  int z = std::stoi(z_input);

  for (int t = 0; t < z; ++t) {
    std::string size_input = take_input(isValidTableSize);
    int n = std::stoi(size_input);

    Table table = input_table(n);
    Anwser answer = kadane(table);

    if (answer.sum == 0) {
      std::cout << "-1 -1 0\n";
    } else {
      std::cout << answer.start << " " << answer.end << " " << answer.sum
                << "\n";
    }
  }

  return 0;
}
