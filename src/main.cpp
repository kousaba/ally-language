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
  )";
  std::cout << source << std::endl;
  ally::Lexer lexer(source);
  auto tokens = lexer.tokenize();
  lexer.printTokens();

  return 0;
}
