#include "ally/ast/expr.h"
#include <iostream>

namespace ally::ast {
void NumberLiteralNode::dump(int indent) {
  printIndent(indent);
  std::cout << "NumberLiteralNode: " << number << std::endl;
}
void BinaryOpNode::dump(int indent) {
  printIndent(indent);
  std::cout << "BinaryOpNode: " << std::endl;
  printIndent(indent + 1);
  std::cout << "Left: " << std::endl;
  lhs->dump(indent + 2);
  printIndent(indent + 1);
  std::cout << "Right: " << std::endl;
  rhs->dump(indent + 2);
  printIndent(indent + 1);
  std::cout << "Op: " << op << std::endl;
}
void VariableRefNode::dump(int indent) {
  printIndent(indent);
  std::cout << "VariableRefNode: " << varName << std::endl;
}
} // namespace ally::ast
