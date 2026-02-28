#include "ally/ast/function.h"

namespace ally::ast {
void Arg::dump(int indent) {
  printIndent(indent);
  std::cout << "Arg: " << name << std::endl;
}
void FunctionNode::dump(int indent) {
  printIndent(indent);
  std::cout << "FunctionNode: (" << std::endl;
  printIndent(indent + 1);
  std::cout << "FuncName: " << functionName << std::endl;
  printIndent(indent + 1);
  std::cout << "Args: (" << std::endl;
  for (auto &i : args)
    i.dump(indent + 2);
  printIndent(indent + 1);
  std::cout << ")" << std::endl;
  printIndent(indent + 1);
  std::cout << "Block: " << std::endl;
  block->dump(2);
  printIndent(indent + 1);
  std::cout << ")" << std::endl;
  printIndent(indent);
  std::cout << ")" << std::endl;
}
} // namespace ally::ast
