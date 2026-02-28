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
  return 5;
}
  )";
  ally::Lexer lexer(source);
  auto tokens = lexer.tokenize();
  lexer.printTokens();
  ally::Parser parser(tokens);
  std::cout << "Parser parsing...." << std::endl;
  auto ast = parser.parse();
  std::cout << "AST Func count: " << ast.size() << std::endl;
  for (auto &i : ast)
    i->dump(0);
  std::cout << "Semantic anaylsis...." << std::endl;
  ally::sema::SemanticAnalysis sema(std::move(ast));
  auto hir = sema.analysis();
  ally::sema::SymbolTable::getInstance().dump();
  ally::mir::MIRBuilder mirBuilder(std::move(hir));
  auto mir = mirBuilder.build();
  for (auto &func : mir) {
    func->dump();
  }
  std::cout << std::endl << "Source: ";
  std::cout << source << std::endl;

  ally::error::ErrorHandler::getInstance().printAllErrors();

  return 0;
}
