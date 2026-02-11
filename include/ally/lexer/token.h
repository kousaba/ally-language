#pragma once
#include "ally/location.h"
#include <string>

namespace ally {
enum class TokenType {
  FN,
  RETURN,
  LET,
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
  Location loc;
};
} // namespace ally
