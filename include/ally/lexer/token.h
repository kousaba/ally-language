#pragma once
#include <string>

namespace ally {
enum class TokenType {
  FN,
  RETURN,
  IDENTIFIER,
  NUMBER,
  SEMI,
  LPAREN,
  RPAREN,
  LBRACKET,
  RBRACKET,
  UNKNOWN,
};
struct Token {
  TokenType type;
  std::string value;
  int line;
  int column;
};
} // namespace ally
