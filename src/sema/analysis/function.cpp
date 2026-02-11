#include "ally/ast/function.h"
#include "ally/error/ErrorHandler.h"
#include "ally/sema/Symbol.h"
#include "ally/sema/SymbolTable.h"

namespace ally::ast {
void FunctionNode::firstAnalysis() {
  std::vector<sema::Arg> newArgs;

  bool result = sema::SymbolTable::getInstance().addSymbol(
      functionName,
      sema::FunctionSymbol{std::move(newArgs), Type{TypeInfo::UNKNOWN, 0}});
  if (!result) {
    error::ErrorHandler::getInstance().report(
        error::Code::ERR_SEM_MULTIPLE_FUNCTION_DECL, {functionName});
  }
}
void FunctionNode::secondAnalysis() { block->analysis(); }
Node *FunctionNode::analysis() {
  error::ErrorHandler::getInstance().report(
      error::Code::COM_SEM_FUNCTIONNODE_VISITED, {});
  return nullptr;
}
} // namespace ally::ast
