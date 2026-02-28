#include "ally/mir/node/base.h"
#include "ally/ast/other.h"

namespace ally::mir {
NodeType Node::getType() { return type; }
bool Node::isExpr() { return false; }
bool Node::isStmt() { return false; }

void printIndent(int indent) { ast::printIndent(indent); }

void UnknownNode::dump(int indent) {
  printIndent(indent);
  std::cout << "Unknown()" << std::endl;
}
} // namespace ally::mir
