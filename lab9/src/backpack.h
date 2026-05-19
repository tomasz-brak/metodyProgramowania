#pragma once
#include <format>
#include <fstream>
#include <vector>
class Backpack {
public:
  Backpack() {};
  Backpack(int capacity, int maxQuantity)
      : capacity(capacity), maxQuantity(maxQuantity) {}
  int capacity;
  int maxQuantity;
  struct SimpleItem {
    int mass;
  };

  struct Item : public SimpleItem {
    std::string name;
    int price;
    Item(std::string name, int mass, int price) : name(name), price(price) {
      this->mass = mass;
    }
    Item(SimpleItem i) : price(0), name("") { this->mass = i.mass; }
    Item() : price(0), name("") { mass = 0; }
  };

  struct PackedItem {
    Item i;
    int quantity;
  };

  std::vector<std::vector<int>> packed;
  std::vector<std::vector<int>> packed_idx;
  std::vector<Item> items;
  Backpack(std::ifstream &file);
  void solveIterative();
  static std::vector<Backpack> loadRecursive(std::ifstream &inFile);
  void solveRecursive();
  int recursiveHelper(int i, int j);
  std::vector<int> backtrack();
};
template <> struct std::formatter<Backpack::Item> {
  constexpr auto parse(std::format_parse_context &ctx) { return ctx.begin(); }

  auto format(const Backpack::Item &item, std::format_context &ctx) const {
    return std::format_to(ctx.out(), "[{}, {}kg, {}$]", item.name, item.mass,
                          item.price);
  }
};

template <> struct std::formatter<Backpack::SimpleItem> {
  constexpr auto parse(std::format_parse_context &ctx) { return ctx.begin(); }

  auto format(const Backpack::SimpleItem &item,
              std::format_context &ctx) const {
    return std::format_to(ctx.out(), "[{}]", item.mass);
  }
};

template <> struct std::formatter<Backpack> {
  constexpr auto parse(std::format_parse_context &ctx) { return ctx.begin(); }

  auto format(const Backpack &bp, std::format_context &ctx) const {
    auto out = std::format_to(ctx.out(), "Backpack(capacity: {}, items: [",
                              bp.capacity);

    for (size_t i = 0; i < bp.items.size(); ++i) {
      out = std::format_to(out, "{}", bp.items[i]);
      if (i != bp.items.size() - 1) {
        out = std::format_to(out, ", ");
      }
    }

    return std::format_to(out, "])");
  }
};
