#pragma once
#include "other.h"
#include <cstdint>
#include <memory>

namespace ally::ast {
class ExprNode : public Node {
private:
  Type type;

public:
  Type getType() { return type; }
  void setType(Type val) {
    if (type.info != TypeInfo::UNKNOWN) {
      // multiple setted
    }
    type = val;
  }
  ExprNode(NodeType nodeType, Location loc)
      : Node(nodeType, loc), type(Type(TypeInfo::UNKNOWN, 0)) {}
};
class NumberLiteralNode : public ExprNode {
  int64_t number;

public:
  NumberLiteralNode(int64_t number, Location loc)
      : number(number), ExprNode(NodeType::NUMBERLITERAL, loc) {}
  Node *analysis() override;
  int64_t getNumber() { return number; }
};

class BinaryOpNode : public ExprNode {
  std::unique_ptr<ExprNode> lhs;
  std::unique_ptr<ExprNode> rhs;
  std::string op;

public:
  BinaryOpNode(std::unique_ptr<ExprNode> lhs, std::string op,
               std::unique_ptr<ExprNode> rhs, Location loc)
      : ExprNode(NodeType::BINARYOP, loc), lhs(std::move(lhs)),
        op(std::move(op)), rhs(std::move(rhs)) {}
  Node *analysis() override;
  std::unique_ptr<ExprNode> getLhs() { return std::move(lhs); }
  std::unique_ptr<ExprNode> getRhs() { return std::move(rhs); }
  std::string getOp() { return op; }
};
} // namespace ally::ast
