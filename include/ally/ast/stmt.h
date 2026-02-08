#pragma once
#include "expr.h"
#include "other.h"
#include <memory>

namespace ally::ast {
class StmtNode : public Node {
public:
  using Node::Node;
};
class ReturnNode : public StmtNode {
  std::unique_ptr<ExprNode> value;

public:
  ReturnNode(std::unique_ptr<ExprNode> value, Location loc)
      : value(std::move(value)), StmtNode(NodeType::RETURN, loc) {}
};
class BlockNode : public StmtNode {
  std::vector<std::unique_ptr<StmtNode>> statements;

public:
  BlockNode(std::vector<std::unique_ptr<StmtNode>> stmts, Location loc)
      : statements(std::move(stmts)), StmtNode(NodeType::BLOCK, loc) {}
};

} // namespace ally::ast
