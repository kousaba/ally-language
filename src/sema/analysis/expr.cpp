#include "ally/ast/expr.h"

namespace ally::ast {
Node *BinaryOpNode::analysis() {
  setType(Type(TypeInfo::INT));
  return this;
}
Node *NumberLiteralNode::analysis() {
  setType(Type(TypeInfo::INT));
  return this;
}
} // namespace ally::ast
