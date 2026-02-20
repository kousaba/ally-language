#include "ally/mir/function.h"

namespace ally::mir {
void Function::dump() {
  std::cout << "Function " << funcName << "(" << std::endl;
  for (auto &i : blocks) {
    i->dump(1);
  }
  std::cout << ")" << std::endl;
  ;
}
} // namespace ally::mir
