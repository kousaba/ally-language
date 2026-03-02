#pragma once
#include "ally/ast/other.h"
#include "ally/mir/node/expr.h"
#include "base.h"
#include <memory>
#include <string>

namespace ally::mir {
class StmtNode : public Node {
public:
  using Node::Node;
  bool isStmt() override;
};
class AssignmentNode : public StmtNode {
  std::string varName;
  ast::Type varType;
  std::unique_ptr<ExprNode> value;

public:
  AssignmentNode(std::string var, ast::Type varType,
                 std::unique_ptr<ExprNode> val);
  void dump(int indent) override;
};
class AllocaNode : public StmtNode{
  std::string varName;
  ast::Type type;
public:
  AllocaNode(std::string var, ast::Type t);
  void dump(int indent) override;
};
class StoreNode : public StmtNode{
  std::string varName;
  std::unique_ptr<ExprNode> value;
public:
  StoreNode(std::string var, std::unique_ptr<ExprNode> expr);
  void dump(int indent) override;
};

} // namespace ally::mir
