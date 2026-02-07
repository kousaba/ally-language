#pragma once
#include "expr.h"
#include "other.h"

namespace ally::ast {
class StmtNode : public Node {
public:
  using Node::Node;
};
class ReturnNode : public StmtNode {
  ExprNode value;

public:
  ReturnNode(ExprNode value, int line, int column)
      : value(value), StmtNode(NodeType::RETURN, line, column) {}
};

} // namespace ally::ast
