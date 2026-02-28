#include "ally/ast/stmt.h"
#include "ally/error/ErrorHandler.h"
#include "ally/sema/Symbol.h"
#include "ally/sema/SymbolTable.h"

namespace ally::ast {
Node *ReturnNode::analysis() {
  // TODO: functionの戻り値と同じ型かチェック
  auto node = value->analysis();
  if (auto expr = dynamic_cast<ExprNode *>(node)) {
    Type exprType = expr->getType();
  } else {
    error::ErrorHandler::getInstance().report(
        error::Code::COM_SEM_EXPECTED_EXPR, {});
  }
  return this;
}
Node *LetNode::analysis() {
  if (sema::SymbolTable::getInstance().lookupSymbol(varName)) {
    error::ErrorHandler::getInstance().report(
        error::Code::ERR_SEM_VAR_MULTIPLE_DEFINED, {varName});
  }
  if (initExpr) {
    auto node = initExpr->analysis();
    if (auto expr = dynamic_cast<ExprNode *>(node)) {
      Type initType = expr->getType();
      if (varType != initType) {
        error::ErrorHandler::getInstance().report(
            error::Code::ERR_SEM_VAR_NOT_MATCH_VALUE_TYPE, {varName});
      }
    } else {
      error::ErrorHandler::getInstance().report(
          error::Code::COM_SEM_EXPECTED_EXPR, {});
    }
  }
  sema::SymbolTable::getInstance().addSymbol(
      varName, sema::VarSymbol{varType, isMutable});
  return this;
}
Node *BlockNode::analysis() {
  sema::SymbolTable::getInstance().addScope();
  for (auto &&stmt : statements) {
    stmt->analysis();
  }
  sema::SymbolTable::getInstance().removeScope();
  return this;
}
} // namespace ally::ast
