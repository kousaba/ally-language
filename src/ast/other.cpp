#include "ally/ast/other.h"
#include <iostream>

namespace ally::ast {
void printIndent(int indent) {
  for (int i = 0; i < indent; i++) {
    std::cout << "  ";
  }
}
} // namespace ally::ast
