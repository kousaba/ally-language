#pragma once
#include "other.h"
#include <cstdint>

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
};
} // namespace ally::ast
