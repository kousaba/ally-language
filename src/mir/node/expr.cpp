#include "ally/mir/node/expr.h"

namespace ally::mir {
bool ExprNode::isExpr() { return true; }
BinaryOpNode::BinaryOpNode(std::unique_ptr<ExprNode> lhs,
                           std::unique_ptr<ExprNode> rhs, Op op)
    : ExprNode(NodeType::BINARYOP), lhs(std::move(lhs)), rhs(std::move(rhs)),
      op(op) {}
ValueNode::ValueNode(std::unique_ptr<Value> val)
    : ExprNode(NodeType::VALUE), val(std::move(val)) {}
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
  }
  return "UNKNOWN OP";
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
} // namespace ally::mir
