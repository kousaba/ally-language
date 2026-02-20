#include "ally/mir/node/base.h"

namespace ally::mir {
NodeType Node::getType() { return type; }
bool Node::isExpr() { return false; }
bool Node::isStmt() { return false; }

void printIndent(int indent) {
  for (int i = 0; i < indent; i++)
    std::cout << "  ";
}

void UnknownNode::dump(int indent) {
  printIndent(indent);
  std::cout << "Unknown()" << std::endl;
}
} // namespace ally::mir
