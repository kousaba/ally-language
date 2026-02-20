#pragma once
#include "ally/ast/other.h"
#include "ally/mir/value.h"
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
  std::unique_ptr<Value> value;

public:
  AssignmentNode(std::string var, ast::Type varType,
                 std::unique_ptr<Value> val);
  void dump(int indent) override;
};
} // namespace ally::mir
