#include "ally/sema/sema.h"
#include "ally/sema/SymbolTable.h"
#include <iostream>

namespace ally::sema {
std::vector<std::unique_ptr<ast::FunctionNode>> SemanticAnalysis::analysis() {
  SymbolTable::getInstance().addScope();
  firstPath();
  secondPath();
  return std::move(ast);
}
void SemanticAnalysis::firstPath() {
  for (size_t i = 0; i < ast.size(); i++) {
    if (!ast[i]) {
      std::cout << "Element " << i << " is NULL!" << std::endl;
      continue;
    }
    std::cout << "Analyzing element " << i << " at " << ast[i].get()
              << std::endl;
    ast[i]->firstAnalysis();
  }
  /*for (auto &&a : ast) {
    a->firstAnalysis();
  }*/
}
void SemanticAnalysis::secondPath() {
  for (auto &&a : ast) {
    a->secondAnalysis();
  }
}
} // namespace ally::sema
