#include "containers/OneWayStack.h"
#include "containers/list.h"
#include "logging.h"
#include <algorithm>
#include <exception>
#include <format>
#include <initializer_list>
#include <map>
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

class Operation : public EquationElement {
  std::shared_ptr<Operator> op;
  std::shared_ptr<EquationElement> left;
  std::shared_ptr<EquationElement> right;

public:
  Operation(std::shared_ptr<Operator> op, std::shared_ptr<EquationElement> left,
            std::shared_ptr<EquationElement> right)
      : op(op), left(left), right(right) {};
  std::string getItem() const {
    return std::format("({} {} {})", left->getItem(), op->getItem(),
                       right->getItem());
  }
  std::shared_ptr<EquationElement> getLeft() { return this->left; }
  std::shared_ptr<EquationElement> getRight() { return this->right; }
  std::shared_ptr<Operator> getOperator() { return this->op; }
};

class UnresolvedToken : public Operator {
  using Operator::Operator;
};
struct OpMetadata {
  int precedence;
  bool rightAssoc;
  bool isSingleActing = false;
};

const std::map<std::string, OpMetadata> PRECEDENCE_MAP = {
    {"=", {0, true}},

    {"<", {1, false}}, {">", {1, false}},

    {"+", {2, false}}, {"-", {2, false}},

    {"*", {3, false}}, {"/", {3, false}}, {"%", {3, false}},

    {"^", {4, true}},

    {"~", {5, true}}};

class Equation {
public:
  Equation() {}
  std::shared_ptr<Operation> RootOp = nullptr;

  /**
   * @brief Returns a representation of the equasion in RPN.
   *
   * Performes a LRN(post order) traversal of the equasion tree.
   *
   */
  std::string toRPN() {

    std::string result;
    OneWayStack<std::shared_ptr<EquationElement>> toVisit = {RootOp};

    while (!toVisit.isEmpty()) {
      auto current = toVisit.pop();

      if (std::shared_ptr<Operation> op =
              std::dynamic_pointer_cast<Operation>(current);
          op) {
        toVisit.put(op->getLeft());
        toVisit.put(op->getRight());
        result += op->getOperator()->getItem();
      } else if (std::shared_ptr<Operand> op =
                     std::dynamic_pointer_cast<Operand>(current);
                 op) {
        result += op->getItem();
      } else {
        Logger::warn("Was unable to find a sufficient match for part of the "
                     "equasion tree with value = {}",
                     current->getItem());
      }
    }
    std::reverse(result.begin(), result.end());
    return result;
  }

  std::string toIFX() { return this->RootOp->getItem(); }

  void parseToTree(const std::initializer_list<std::string> &tokens) {
    OneWayStack<std::shared_ptr<Operator>> ops({});
    OneWayStack<std::shared_ptr<EquationElement>> nodes({});

    // pop an operator and build a node in the tree
    auto processOp = [&]() {
      if (ops.isEmpty())
        return;

      auto op = ops.pop();

      if (op->getItem() == "~") {
        if (nodes.isEmpty())
          return;
        auto operand = nodes.pop();
        // Create operation with null left child for unary
        nodes.put(std::make_shared<Operation>(op, nullptr, operand));
      }
      // Handle Binary Operators
      else {
        if (nodes.isEmpty())
          return;
        auto right = nodes.pop();
        if (nodes.isEmpty())
          return;
        auto left = nodes.pop();
        nodes.put(std::make_shared<Operation>(op, left, right));
      }
    };

    for (const std::string &token : tokens) {
      // Parentheses handling
      if (token == "(") {
        ops.put(std::make_shared<Operator>("("));
      } else if (token == ")") {
        while (!ops.isEmpty() && ops.top()->getItem() != "(") {
          processOp();
        }
        if (!ops.isEmpty())
          ops.pop();
      }
      // Operator handling
      else if (PRECEDENCE_MAP.contains(token)) {
        const auto &currentOp = PRECEDENCE_MAP.at(token);

        while (!ops.isEmpty() && ops.top()->getItem() != "(") {
          const std::string &topToken = ops.top()->getItem();
          const auto &topOp = PRECEDENCE_MAP.at(topToken);

          // Shunting-yard pop logic
          bool shouldPop = (topOp.precedence > currentOp.precedence) ||
                           (topOp.precedence == currentOp.precedence &&
                            !currentOp.rightAssoc);

          if (shouldPop) {
            processOp();
          } else {
            break;
          }
        }
        ops.put(std::make_shared<Operator>(token));
      }
      // Operand handling
      else {
        nodes.put(std::make_shared<Operand>(token));
      }
    }

    // Process remaining operators in stack
    while (!ops.isEmpty()) {
      processOp();
    }

    if (!nodes.isEmpty()) {
      this->RootOp = std::dynamic_pointer_cast<Operation>(nodes.pop());
    }
  }

  template <std::ranges::range R> void parseFromRPN(R &tokens) {
    auto previousOperations = OneWayStack<std::shared_ptr<EquationElement>>({});

    for (const auto &token : tokens) {
      std::string tokenStr(1, token);

      if (PRECEDENCE_MAP.contains(tokenStr)) {
        auto opt = std::make_shared<Operator>(tokenStr);
        const bool isSingleActing = PRECEDENCE_MAP.at(tokenStr).isSingleActing;

        if (isSingleActing) {
          if (previousOperations.isEmpty()) {
            Logger::error("Operands didn't match the amount of operators");
            break;
          }
          auto rightSide = previousOperations.pop();
          previousOperations.put(
              std::make_shared<Operation>(opt, nullptr, rightSide));
        } else {
          if (previousOperations.size() < 2) {
            Logger::error("Operands didn't match the amount of operators");
            break;
          }
          auto rightSide = previousOperations.pop();
          auto leftSide = previousOperations.pop();
          previousOperations.put(
              std::make_shared<Operation>(opt, leftSide, rightSide));
        }
      } else {
        previousOperations.put(std::make_shared<Operand>(tokenStr));
      }
    }

    if (!previousOperations.isEmpty()) {
      RootOp = std::dynamic_pointer_cast<Operation>(previousOperations.pop());
    }
  }
  Equation copy() { throw std::runtime_error("Not implemented"); }
};

template <> struct std::formatter<Equation, char> {
  constexpr auto parse(std::format_parse_context &ctx) {
    auto it = ctx.begin();
    return it;
  }

  auto formatTree(shared_ptr<Operation> rootToken,
                  std::format_context::iterator &out) const {
    OneWayStack<const shared_ptr<Operation>> elements({});
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
