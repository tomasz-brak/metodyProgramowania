#include "Equasion.h"

const std::map<std::string, OpMetadata> PRECEDENCE_MAP = {
    {"=", {0, true}},      {"<", {1, false}}, {">", {1, false}},
    {"+", {2, false}},     {"-", {2, false}}, {"*", {3, false}},
    {"/", {3, false}},     {"%", {3, false}}, {"^", {4, true}},
    {"~", {5, true, true}}};

std::string Operand::getItem() const { return this->_item; }

std::unique_ptr<Operand> Operand::copy() const {
  throw std::runtime_error("Not implemented");
}

std::string Operator::getItem() const { return this->_item; }

std::unique_ptr<Operator> Operator::copy() const {
  throw std::runtime_error("Not implemented");
}

Operation::Operation(std::shared_ptr<Operator> op,
                     std::shared_ptr<EquationElement> left,
                     std::shared_ptr<EquationElement> right)
    : op(op), left(left), right(right) {}

std::string Operation::getItem() const {
  auto getChildItem = [&](std::shared_ptr<EquationElement> child) {
    auto childOp = std::dynamic_pointer_cast<Operation>(child);
    if (!childOp)
      return child->getItem();

    int myPrec = PRECEDENCE_MAP.at(this->op->getItem()).precedence;
    int childPrec =
        PRECEDENCE_MAP.at(childOp->getOperator()->getItem()).precedence;

    if (childPrec <= myPrec) {
      return std::format("({})", childOp->getItem());
    }
    return childOp->getItem();
  };

  std::string leftStr = left ? getChildItem(left) : "";
  std::string rightStr = getChildItem(right);

  if (!left)
    return std::format("{}{}", op->getItem(), rightStr);
  return std::format("{} {} {}", leftStr, op->getItem(), rightStr);
}

std::shared_ptr<EquationElement> Operation::getLeft() { return this->left; }
std::shared_ptr<EquationElement> Operation::getRight() { return this->right; }
std::shared_ptr<Operator> Operation::getOperator() { return this->op; }

Equation::Equation() {}

std::string Equation::toRPN() {
  if (!RootOp)
    return "";

  List<std::string> tokens;
  OneWayStack<std::shared_ptr<EquationElement>> stack({});
  stack.put(RootOp);

  while (!stack.isEmpty()) {
    auto current = stack.pop();
    if (!current)
      continue;

    if (auto op = std::dynamic_pointer_cast<Operation>(current)) {
      tokens.append(op->getOperator()->getItem());
      stack.put(op->getLeft());
      stack.put(op->getRight());
    } else {
      tokens.append(current->getItem());
    }
  }

  std::reverse(tokens.begin(), tokens.end());

  std::string result;
  for (const auto &t : tokens)
    result += t;
  return result;
}

std::string Equation::toIFX() {
  if (!RootOp)
    return "";
  return this->RootOp->getItem();
}
