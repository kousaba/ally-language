#include "ally/ast/expr.h"
#include "ally/sema/SymbolTable.h"
#include <iostream>

namespace ally::ast {

Node *BinaryOpNode::analysis() {
  setType(Type(TypeInfo::INT));
  return this;
}
Node *NumberLiteralNode::analysis() {
  setType(Type(TypeInfo::INT));
  return this;
}
Node *VariableRefNode::analysis() {
  if (sema::Symbol symbol =
          sema::SymbolTable::getInstance().lookupSymbol(varName)) {
    if (symbol.type != sema::SymbolType::VAR) {
      // TODO: Error 変数ではない
      std::cerr << "Not variable: " << varName << std::endl;
      return nullptr;
    }
    if (auto varSymbol = std::get_if<sema::VarSymbol>(&symbol.symbol)) {
      setType(varSymbol->varType);
    }
  } else {
    return nullptr;
  }
  return this;
}
} // namespace ally::ast
