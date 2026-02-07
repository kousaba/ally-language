#pragma once
#include "ally/lexer/token.h"
#include <cstddef>
#include <optional>
#include <string>
#include <vector>

namespace ally {
class Lexer {
private:
  size_t pos;
  int line, column;
  std::string source;
  void skipWhitespace();
  std::optional<Token> nextToken();
  Token parseNumber();
  Token parseWords();
  Token parseSymbols();
  bool isAtEnd();
  char peek(int offset = 0);
  char advance();

  Token makeToken(TokenType type, std::string value);

public:
  Lexer(std::string src) : source(src), pos(0), line(0), column(0) {}
  std::vector<Token> tokenize();
  void printTokens();
};
} // namespace ally
