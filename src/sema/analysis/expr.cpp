#include "ally/ast/expr.h"
#include "ally/error/ErrorHandler.h"
#include "ally/sema/SymbolTable.h"

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
      error::ErrorHandler::getInstance().report(
          error::Code::ERR_SEM_SYMBOL_IS_NOT_VAR, {varName});
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
