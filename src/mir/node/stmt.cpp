#include "ally/mir/node/stmt.h"

namespace ally::mir {
bool StmtNode::isStmt() { return true; }

AssignmentNode::AssignmentNode(std::string var, ast::Type varType,
                               std::unique_ptr<ExprNode> val)
    : StmtNode(NodeType::ASSIGNMENT), varName(std::move(var)), varType(varType),
      value(std::move(val)) {}
AllocaNode::AllocaNode(std::string var, ast::Type t)
    : StmtNode(NodeType::ALLOCA), varName(std::move(var)), type(t) {}
StoreNode::StoreNode(std::string var, std::unique_ptr<ExprNode> val)
    : StmtNode(NodeType::STORE), varName(std::move(var)),
      value(std::move(val)) {}

void AssignmentNode::dump(int indent) {
  printIndent(indent);
  std::cout << "AssignmentNode(" << std::endl;
  value->dump(indent + 1);
  printIndent(indent + 1);
  std::cout << "Var: " << varName << std::endl;
  printIndent(indent);
  std::cout << ")" << std::endl;
}

void AllocaNode::dump(int indent) {
  printIndent(indent);
  std::cout << "AllocaNode(" << std::endl;
  printIndent(indent + 1);
  std::cout << "Var: " << varName << std::endl;
  printIndent(indent);
  std::cout << ")" << std::endl;
}
void StoreNode::dump(int indent) {
  printIndent(indent);
  std::cout << "StoreNode(" << std::endl;
  value->dump(indent + 1);
  printIndent(indent + 1);
  std::cout << "Var: " << varName << std::endl;
  printIndent(indent);
  std::cout << ")" << std::endl;
}

} // namespace ally::mir
