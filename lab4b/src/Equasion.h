#include "containers/OneWayStack.h"
#include "containers/list.h"
#include <exception>
#include <format>
#include <memory>
#include <stdexcept>
#include <string>

class Operator;

template <typename T> using sList = List<std::shared_ptr<T>>;

class EquationElement {
public:
  EquationElement(const std::string &s) { this->_item = s; }
  EquationElement() = default;
  virtual ~EquationElement() = default;
  virtual std::string getItem() const = 0;

protected:
  std::string _item = "empty";
};

class Operand : public EquationElement {
public:
  std::string getItem() const override { return this->_item; }
  using EquationElement::EquationElement;

  std::unique_ptr<Operand> copy() const {
    throw std::runtime_error("Not implemented");
  };
};

class Operator : public EquationElement {
public:
  std::string getItem() const override { return this->_item; }
  using EquationElement::EquationElement;

  std::unique_ptr<Operator> copy() const {
    throw std::runtime_error("Not implemented");
  }
};

class Operation : EquationElement {
  Operation();
  std::shared_ptr<Operator> op;
  std::shared_ptr<EquationElement> left;
  std::shared_ptr<EquationElement> right;
};

class UnresolvedToken : public Operator {
  using Operator::Operator;
};

class Equation {
public:
  Equation() {}
  std::shared_ptr<Operation> RootOp = nullptr;

  Equation copy() { throw std::runtime_error("Not implemented"); }
};

template <> struct std::formatter<Equation, char> {
  constexpr auto parse(std::format_parse_context &ctx) {
    auto it = ctx.begin();
    return it;
  }

  auto formatTree(shared_ptr<Operator> rootToken,
                  std::format_context::iterator &out) const {
    OneWayStack<const shared_ptr<EquationElement>> elements({});
    elements.put(rootToken);

    while (!elements.isEmpty()) {
      const auto current = elements.pop();

      std::format_to(out, "{}", std::string(current->getItem()));
    }
    return out;
  }

  template <typename FormatContext>
  auto format(const Equation &eq, FormatContext &ctx) const {
    typename FormatContext::iterator out = ctx.out();
    std::format_to(out, "Equation\n");

    return formatTree(eq.RootOp, out);
  }
};
