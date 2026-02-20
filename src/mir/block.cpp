#include "ally/mir/block.h"
#include "ally/mir/node/base.h"
#include <iostream>

namespace ally::mir {
void Block::dump(int indent) {
  printIndent(indent);
  std::cout << "Block " << blockName << "(" << std::endl;
  for (auto &stmt : statements) {
    stmt->dump(indent + 1);
  }
  terminator->dump(indent + 1);
  printIndent(indent);
  std::cout << ")" << std::endl;
}
} // namespace ally::mir
