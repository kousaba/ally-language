#include "ally/mir/value.h"
#include "ally/mir/node/base.h"
#include <iostream>

namespace ally::mir {
void ConstantValue::dump(int indent) {
  printIndent(indent);
  std::cout << "ConstantValue: ";
  std::visit([&](auto &&arg) { std::cout << arg; }, value);
  std::cout << std::endl;
}
void VariableValue::dump(int indent) {
  printIndent(indent);
  std::cout << "VariableValue: (" << std::endl;
  printIndent(indent + 1);
  std::cout << "Var: " << varName << std::endl;
  printIndent(indent);
  std::cout << ")" << std::endl;
}
} // namespace ally::mir
