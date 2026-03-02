#include "ally/ast/stmt.h"
#include <iostream>

namespace ally::ast {
void ReturnNode::dump(int indent) {
  printIndent(indent);
  std::cout << "ReturnNode: " << std::endl;
  printIndent(indent + 1);
  std::cout << "Value: " << std::endl;
  value->dump(indent + 2);
}
void LetNode::dump(int indent) {
  printIndent(indent);
  std::cout << "LetNode: " << std::endl;
  printIndent(indent + 1);
  std::cout << "VarName: " << varName << std::endl;
  printIndent(indent + 1);
  std::cout << "init: " << std::endl;
  if (initExpr)
    initExpr->dump(indent + 2);
}
void BlockNode::dump(int indent) {
  printIndent(indent);
  std::cout << "BlockNode: " << std::endl;
  for (auto &i : statements) {
    i->dump(indent + 1);
  }
}
void IfNode::dump(int indent) {
  printIndent(indent);
  std::cout << "IfNode: " << std::endl;
  printIndent(indent + 1);
  std::cout << "Condition: " << std::endl;
  cond->dump(indent + 2);
  printIndent(indent + 1);
  std::cout << "thenB: " << std::endl;
  thenB->dump(indent + 2);
  printIndent(indent + 1);
  std::cout << "elseB: " << std::endl;
  if (elseB)
    elseB->dump(indent + 2);
  else {
    printIndent(indent + 2);
    std::cout << "nullptr" << std::endl;
  }
}
} // namespace ally::ast
