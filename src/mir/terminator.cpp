#include "ally/mir/terminator.h"
#include "ally/mir/node/base.h"

namespace ally::mir {
void ReturnTerminator::dump(int indent) {
  printIndent(indent);
  std::cout << "ReturnTerm: (" << std::endl;
  returnValue->dump(indent + 1);
  printIndent(indent);
  std::cout << ")" << std::endl;
}
void JumpTerminator::dump(int indent) {
  printIndent(indent);
  std::cout << "JumpTerm"
            << std::endl; // class Block;にしているのでメンバ関数を呼べない
}
void IfTerminator::dump(int indent) {
  printIndent(indent);
  std::cout << "IfTerminator: (" << std::endl;
  cond->dump(indent + 1);
  // TODO: thenB->getBlockName(), elseB->getBlockName()を出力
  std::cout << ")" << std::endl;
}
void UnreachableTerminator::dump(int indent) {
  printIndent(indent);
  std::cout << "UnreachableTerminator" << std::endl;
}
} // namespace ally::mir
