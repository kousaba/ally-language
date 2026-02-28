#include "ally/ast/stmt.h"
#include <iostream>

namespace ally::ast {
void ReturnNode::dump(int indent) {
  printIndent(indent);
  std::cout << "ReturnNode: (" << std::endl;
  printIndent(indent + 1);
  std::cout << "Value: (" << std::endl;
  value->dump(indent + 2);
  printIndent(indent + 1);
  std::cout << ")" << std::endl;
  printIndent(indent);
  std::cout << ")" << std::endl;
}
void LetNode::dump(int indent) {
  printIndent(indent);
  std::cout << "LetNode: (" << std::endl;
  printIndent(indent + 1);
  std::cout << "VarName: " << varName << std::endl;
  printIndent(indent + 1);
  std::cout << "init: (" << std::endl;
  if (initExpr)
    initExpr->dump(indent + 2);
  printIndent(indent + 1);
  std::cout << ")" << std::endl;
  printIndent(indent);
  std::cout << ")" << std::endl;
}
void BlockNode::dump(int indent) {
  printIndent(indent);
  std::cout << "BlockNode: (" << std::endl;
  for (auto &i : statements) {
    i->dump(indent + 1);
  }
  printIndent(indent);
  std::cout << ")" << std::endl;
}
} // namespace ally::ast
