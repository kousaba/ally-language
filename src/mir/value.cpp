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
} // namespace ally::mir
