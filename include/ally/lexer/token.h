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
  EQ,
  UNKNOWN,
  ADD,
  SUB,
  MUL,
  DIV,
  GT,
  GTE,
  LT,
  LTE,
  EQEQ,
  NEQ,
  IF,
  ELSE,
};
struct Token {
  TokenType type;
  std::string value;
  Location loc;
};
} // namespace ally
