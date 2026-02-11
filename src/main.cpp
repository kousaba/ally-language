#include "ally/error/ErrorCode.h"
#include "ally/error/ErrorHandler.h"
#include "ally/error/ErrorMessage.h"
#include "ally/lexer/lexer.h"
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
  let a
  return 5
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
  sema.analysis();
  ally::sema::SymbolTable::getInstance().dump();
  ally::error::ErrorHandler::getInstance().printAllErrors();

  return 0;
}
