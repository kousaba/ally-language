#pragma once
#include "ally/mir/value.h"
#include "base.h"
#include <memory>

namespace ally::mir {
class ExprNode : public Node {
public:
  using Node::Node;
  bool isExpr() override;
};
class ValueNode : public ExprNode {
  std::unique_ptr<Value> val;

public:
  ValueNode(std::unique_ptr<Value> val);
  void dump(int indent) override;
};
class BinaryOpNode : public ExprNode {
public:
  enum class Op { ADD, SUB, MUL, DIV, EQEQ, NEQ, LT, LTE, GT, GTE };

private:
  std::unique_ptr<ExprNode> lhs;
  std::unique_ptr<ExprNode> rhs;
  Op op;

public:
  BinaryOpNode(std::unique_ptr<ExprNode> lhs, std::unique_ptr<ExprNode> rhs,
               Op op);
  std::string getOpName();
  void dump(int indent) override;
};
} // namespace ally::mir
