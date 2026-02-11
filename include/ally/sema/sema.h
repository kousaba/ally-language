#pragma once
#include "ally/ast/function.h"

namespace ally::sema {
class SemanticAnalysis {
private:
  std::vector<std::unique_ptr<ast::FunctionNode>> ast;
  void firstPath();  // 関数登録
  void secondPath(); // 関数の中のもの
public:
  void analysis();
  SemanticAnalysis(std::vector<std::unique_ptr<ast::FunctionNode>> ast)
      : ast(std::move(ast)) {}
};
} // namespace ally::sema
