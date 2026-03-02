#include "ally/mir/node/expr.h"

namespace ally::mir {
bool ExprNode::isExpr() { return true; }
BinaryOpNode::BinaryOpNode(std::unique_ptr<ExprNode> lhs,
                           std::unique_ptr<ExprNode> rhs, Op op)
    : ExprNode(NodeType::BINARYOP), lhs(std::move(lhs)), rhs(std::move(rhs)),
      op(op) {}
ValueNode::ValueNode(std::unique_ptr<Value> val)
    : ExprNode(NodeType::VALUE), val(std::move(val)) {}
LoadNode::LoadNode(std::string var, ast::Type t)
    : ExprNode(NodeType::LOAD), varName(std::move(var)), type(t) {}
void ValueNode::dump(int indent) {
  printIndent(indent);
  std::cout << "ValueNode(" << std::endl;
  val->dump(indent + 1);
  printIndent(indent);
  std::cout << ")" << std::endl;
}
std::string BinaryOpNode::getOpName() {
  switch (op) {
  case Op::ADD:
    return "ADD";
  case Op::SUB:
    return "SUB";
  case Op::MUL:
    return "MUL";
  case Op::DIV:
    return "DIV";
  case Op::LT:
    return "LT";
  case Op::LTE:
    return "LTE";
  case Op::GT:
    return "GT";
  case Op::GTE:
    return "GTE";
  case Op::EQEQ:
    return "EQEQ";
  case Op::NEQ:
    return "NEQ";
  default:
    return "Unknown Op";
  }
}
void BinaryOpNode::dump(int indent) {
  printIndent(indent);
  std::cout << "BinaryOpNode(" << std::endl;
  printIndent(indent + 1);
  std::cout << "Op: " << getOpName() << std::endl;
  lhs->dump(indent + 1);
  rhs->dump(indent + 1);
  printIndent(indent);
  std::cout << ")" << std::endl;
}
void LoadNode::dump(int indent) {
  printIndent(indent);
  std::cout << "LoadNode(" << std::endl;
  printIndent(indent + 1);
  std::cout << "Var: " << varName << std::endl;
  printIndent(indent);
  std::cout << ")" << std::endl;
}
} // namespace ally::mir
