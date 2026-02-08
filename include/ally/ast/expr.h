#pragma once
#include "other.h"
#include <cstdint>

namespace ally::ast {
class ExprNode : public Node {
public:
  Type type;
  using Node::Node;
};
class NumberLiteralNode : public ExprNode {
  int64_t number;

public:
  NumberLiteralNode(int64_t number, Location loc)
      : number(number), ExprNode(NodeType::NUMBERLITERAL, loc) {}
};
} // namespace ally::ast
