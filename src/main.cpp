#include "ally/error/ErrorCode.h"
#include "ally/error/ErrorHandler.h"
#include "ally/error/ErrorMessage.h"
#include "ally/lexer/lexer.h"
#include <iostream>

int main(int argc, char *argv[]) {
  std::cout << "This is ally compiler!" << std::endl;
  ally::error::ErrorMessageManager::getInstance().setLanguage(
      ally::error::Language::ja);
  std::string source = R"(
fn calc(){
  return 5;
}
fn calc2(){
  return 3;
}
  )";
  ally::Lexer lexer(source);
  auto tokens = lexer.tokenize();
  lexer.printTokens();
  ally::error::ErrorHandler::getInstance().printAllErrors();

  return 0;
}
