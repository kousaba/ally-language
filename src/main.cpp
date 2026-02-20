#include "ally/error/ErrorHandler.h"
#include "ally/error/ErrorMessage.h"
#include "ally/lexer/lexer.h"
#include "ally/mir/builder.h"
#include "ally/parser/parser.h"
#include "ally/sema/SymbolTable.h"
#include "ally/sema/sema.h"
#include <iostream>

int main(int argc, char *argv[]) {
  std::cout << "This is ally compiler!" << std::endl;
  ally::error::ErrorMessageManager::getInstance().setLanguage(
      ally::error::Language::ja);
  std::string source = R"(
fn calc(){
  let a = 5
  return 5
}
fn calc2(){
  let b = 10
  return 3
}
  )";
  ally::Lexer lexer(source);
  auto tokens = lexer.tokenize();
  lexer.printTokens();
  ally::Parser parser(tokens);
  std::cout << "Parser parsing...." << std::endl;
  auto ast = parser.parse();
  std::cout << "Semantic anaylsis...." << std::endl;
  ally::sema::SemanticAnalysis sema(std::move(ast));
  auto hir = sema.analysis();
  ally::sema::SymbolTable::getInstance().dump();
  ally::mir::MIRBuilder mirBuilder(std::move(hir));
  auto mir = mirBuilder.build();
  std::cout << "Function Count: " << mir.size() << std::endl;
  for (auto &func : mir) {
    func->dump();
  }
  ally::error::ErrorHandler::getInstance().printAllErrors();

  return 0;
}
