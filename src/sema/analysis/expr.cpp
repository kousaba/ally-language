#include "ally/ast/expr.h"

namespace ally::ast {
Node *NumberLiteralNode::analysis() {
  setType(Type(TypeInfo::INT));
  return this;
}
} // namespace ally::ast
