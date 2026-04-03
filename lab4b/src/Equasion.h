#pragma once

#include "Exceptions.h"
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

/**
 * @class OpMetadata
 * @brief Metadata of an operator
 */
struct OpMetadata {
  int precedence;
  bool rightAssoc;
  bool isSingleActing = false;
};

/**
 * @brief Precedence map of operators. Containes metadata for allowed operators.
 */
extern const std::map<std::string, OpMetadata> PRECEDENCE_MAP;

template <typename T> using sList = List<std::shared_ptr<T>>;

/**
 * @class EquationElement
 * @brief Element of the equasion tree.
 *
 * Base class for all elements of an equasion tree.
 *
 * Stores the elements position in the original string for error handling.
 */
class EquationElement {
public:
  EquationElement(const std::string &s, int pos = -1)
      : _item(s), position(pos) {}
  EquationElement() = default;
  virtual ~EquationElement() = default;
  virtual std::string getItem() const = 0;

  int position = -1;

protected:
  std::string _item = "empty";
};

/**
 * @class Operand
 * @brief Operand of an Operation
 *
 */
class Operand : public EquationElement {
public:
  std::string getItem() const override;
  using EquationElement::EquationElement;

  std::unique_ptr<Operand> copy() const;
};

/**
 * @class Operator
 * @brief Operator of the equasion
 *
 */
class Operator : public EquationElement {
public:
  std::string getItem() const override;
  using EquationElement::EquationElement;

  std::unique_ptr<Operator> copy() const;
};

/**
 * @class Operation
 * @brief Operation of the equasion
 * Each operation consist of at least one operator and operand. The operand may
 * be another operation
 */
class Operation : public EquationElement {
  std::shared_ptr<Operator> op;
  std::shared_ptr<EquationElement> left;
  std::shared_ptr<EquationElement> right;

public:
  Operation(std::shared_ptr<Operator> op, std::shared_ptr<EquationElement> left,
            std::shared_ptr<EquationElement> right);
  std::string getItem() const override;
  std::shared_ptr<EquationElement> getLeft();
  std::shared_ptr<EquationElement> getRight();
  std::shared_ptr<Operator> getOperator();
};

/**
 * @class Equation
 * @brief Equasion class for representing a mathematical statement.
 *
 * Represents the equasion as a tree graph (AST). With a root operation.
 * @ref RootOp is the first transformation
 *
 */
class Equation {
public:
  Equation();
  std::shared_ptr<Operation> RootOp = nullptr;

  /**
   * @brief Converts the AST into RPN
   * Performs an iterative reverse pre-order traversal of the equasion graph
   * which yields the RPN representation.
   *
   * @return std::string the RPN representation of the AST
   * */

  std::string toRPN();
  /**
   * @brief Converts the AST to IFX
   *
   * Performs a Level order traversal of the graph, recursivly calling the
   * `getName` method. Combines with parenthassis if the previous operation has
   * higher or equal precedence
   */
  std::string toIFX();

  /**
   * @brief Parses a mathematical expression in to the intermediete expression
   * tree.
   *
   * Uses the Shunting Yard Algorithm:
   * reads from left to right until it finds an operator that is higher then the
   * current one. If found pops the stacks writing the output parts into the
   * tree.
   *
   * If parenthassis are present the stack is poped when a closing one occurs.
   * If goes until it finds the opening brace.
   *
   * @param tokens
   */
  template <std::ranges::range R> void parseFromINF(R &tokens) {
    OneWayStack<std::shared_ptr<Operator>> ops({});
    OneWayStack<std::shared_ptr<EquationElement>> nodes({});

    auto processOp = [&](int currentIndex) {
      if (ops.isEmpty())
        return;
      auto op = ops.pop();

      if (op->getItem() == "(") {
        throw ParserError("no matching closing brace", op->position, tokens);
      }

      if (op->getItem() == "~") {
        if (nodes.isEmpty())
          throw ParserError(
              "unable to parse: missing operand for unary operator",
              currentIndex, tokens);
        nodes.put(std::make_shared<Operation>(op, nullptr, nodes.pop()));
      } else {
        if (nodes.isEmpty())
          throw ParserError("unable to parse: missing right operand",
                            currentIndex, tokens);
        auto right = nodes.pop();
        if (nodes.isEmpty())
          throw ParserError("unable to parse: missing left operand",
                            currentIndex, tokens);
        auto left = nodes.pop();
        nodes.put(std::make_shared<Operation>(op, left, right));
      }
    };

    int tokenIndex = 0;
    for (const char &tChar : tokens) {
      auto token = std::string(1, tChar);
      if (token == "(") {
        ops.put(std::make_shared<Operator>("(", tokenIndex));
      } else if (token == ")") {
        while (!ops.isEmpty() && ops.top()->getItem() != "(") {
          processOp(tokenIndex);
        }
        if (ops.isEmpty())
          throw ParserError("unable to parse: no matching opening brace",
                            tokenIndex, tokens);
        ops.pop(); // pop "("
      } else if (PRECEDENCE_MAP.contains(token)) {
        const auto &currentOp = PRECEDENCE_MAP.at(token);
        while (!ops.isEmpty() && ops.top()->getItem() != "(") {
          const auto &topOp = PRECEDENCE_MAP.at(ops.top()->getItem());
          if (topOp.precedence > currentOp.precedence ||
              (topOp.precedence == currentOp.precedence &&
               !currentOp.rightAssoc)) {
            processOp(tokenIndex);
          } else
            break;
        }
        ops.put(std::make_shared<Operator>(token, tokenIndex));
      } else {
        nodes.put(std::make_shared<Operand>(token, tokenIndex));
      }
      tokenIndex++;
    }

    while (!ops.isEmpty())
      processOp(tokenIndex);

    if (nodes.isEmpty())
      return;

    auto finalNode = nodes.pop();
    if (!nodes.isEmpty()) {
      throw ParserError("unable to parse: too many operands", tokenIndex,
                        tokens);
    }

    this->RootOp = std::dynamic_pointer_cast<Operation>(finalNode);
  }

  /**
   * @brief Parses an Infix mathematical expression into an Abstract Syntax
   * Tree.
   *
   * @details Tokens are read from left to right, `Operand`s get put on the
   * stack. `Operator`s pop the required amount of operands, the result of the
   * operation is pushed on to the stack.
   *
   * @param tokens
   */
  template <std::ranges::range R> void parseFromRPN(R &tokens) {
    auto previousOperations = OneWayStack<std::shared_ptr<EquationElement>>({});
    int tokenIndex = 0;

    for (const auto &token : tokens) {
      std::string tokenStr;
      if constexpr (std::is_same_v<std::decay_t<decltype(token)>, char>)
        tokenStr = std::string(1, token);
      else
        tokenStr = std::string(token);

      if (PRECEDENCE_MAP.contains(tokenStr)) {
        auto opt = std::make_shared<Operator>(tokenStr, tokenIndex);
        const bool isSingle = PRECEDENCE_MAP.at(tokenStr).isSingleActing;

        if (isSingle) {
          if (previousOperations.isEmpty())
            throw ParserError("RPN Error: missing operand", tokenIndex, tokens);
          previousOperations.put(std::make_shared<Operation>(
              opt, nullptr, previousOperations.pop()));
        } else {
          if (previousOperations.size() < 2)
            throw ParserError("RPN Error: missing operands for binary operator",
                              tokenIndex, tokens);
          auto right = previousOperations.pop();
          auto left = previousOperations.pop();
          previousOperations.put(std::make_shared<Operation>(opt, left, right));
        }
      } else {
        previousOperations.put(std::make_shared<Operand>(tokenStr, tokenIndex));
      }
      tokenIndex++;
    }

    if (!previousOperations.isEmpty()) {
      auto finalNode = previousOperations.pop();
      if (!previousOperations.isEmpty()) {
        throw ParserError("RPN Error: expected an operator here", tokenIndex,
                          tokens);
      }
      RootOp = std::dynamic_pointer_cast<Operation>(finalNode);
    }
  }
};

template <> struct std::formatter<Equation, char> {
  constexpr auto parse(std::format_parse_context &ctx) {
    auto it = ctx.begin();
    return it;
  }

  auto formatTree(std::shared_ptr<Operation> rootToken,
                  std::format_context::iterator &out) const {
    OneWayStack<const std::shared_ptr<Operation>> elements({});
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
