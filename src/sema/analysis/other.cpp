#include "ally/ast/other.h"

namespace ally::ast {
bool Type::operator==(Type another) {
  return info == another.info && pointer == another.pointer;
}
bool Type::operator!=(Type another) {
  return info != another.info || pointer == another.pointer;
}
} // namespace ally::ast
