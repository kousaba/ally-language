#pragma once
#include "expr.h"
#include "other.h"
#include <memory>
#include <vector>

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
  Node *analysis() override;
  std::unique_ptr<ExprNode> getValue() { return std::move(value); }
};
class LetNode : public StmtNode {
  bool isMutable;
  Type varType;
  std::unique_ptr<ExprNode> initExpr;
  std::string varName;

public:
  LetNode(std::string name, bool ismut, Type type,
          std::unique_ptr<ExprNode> init, Location loc)
      : varName(std::move(name)), isMutable(ismut), varType(type),
        initExpr(std::move(init)), StmtNode(NodeType::LET, loc) {}
  Node *analysis() override;
  bool getIsMutable() { return isMutable; }
  Type getVarType() { return varType; }
  std::unique_ptr<ExprNode> getInitExpr() { return std::move(initExpr); }
  std::string getVarName() { return varName; }
};
class BlockNode : public StmtNode {
  std::vector<std::unique_ptr<StmtNode>> statements;

public:
  BlockNode(std::vector<std::unique_ptr<StmtNode>> stmts, Location loc)
      : statements(std::move(stmts)), StmtNode(NodeType::BLOCK, loc) {}
  Node *analysis() override;
  std::vector<std::unique_ptr<StmtNode>> getStatements() {
    return std::move(statements);
  }
};

} // namespace ally::ast
