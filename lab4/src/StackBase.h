#pragma once
#include <format>
#include <stdexcept>

template <typename T> struct stackItem {
  stackItem<T> *next = nullptr;
  T data;
};

template <typename T> class StackBase {
public:
  StackBase() : first(nullptr), last(nullptr) {}

  virtual ~StackBase() {
    while (first) {
      auto temp = first;
      first = first->next;
      delete temp;
    }
  }

  StackBase(const StackBase &) = delete;
  StackBase &operator=(const StackBase &) = delete;

  virtual int size() = 0;

  bool isEmpty() { return size() == 0; }

  virtual void put(T const &element) = 0;
  virtual T pop() = 0;
  virtual T &top() = 0;

  stackItem<T> *first = nullptr;
  stackItem<T> *last = nullptr;
};

template <typename T, typename CharT>
struct std::formatter<StackBase<T>, CharT> {
  std::formatter<T, CharT> underlying;

  constexpr auto parse(std::format_parse_context &ctx) {
    return underlying.parse(ctx);
  }

  template <typename FormatContext>
  auto format(const StackBase<T> &stack, FormatContext &ctx) const {
    auto out = ctx.out();
    out = std::format_to(out, "[");
    for (auto *current = stack.first; current != nullptr;
         current = current->next) {
      out = underlying.format(current->data, ctx);
      if (current->next != nullptr) {
        out = std::format_to(out, ", ");
      }
    }
    return std::format_to(out, "]");
  }
};
