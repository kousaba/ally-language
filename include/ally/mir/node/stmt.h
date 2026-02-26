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
} // namespace ally::mir
