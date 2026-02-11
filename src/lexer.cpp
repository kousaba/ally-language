#include "ally/lexer/lexer.h"
#include "ally/error/ErrorHandler.h"
#include "ally/lexer/token.h"

#include <iostream>
#include <map>
#include <string>

namespace ally {
std::vector<Token> Lexer::tokenize() {
  pos = 0;
  loc.line = 0;
  loc.column = 0;
  std::vector<Token> tokens;
  while (!isAtEnd()) {
    skipWhitespace();
    auto result = nextToken();
    if (result) {
      tokens.push_back(*result);
    }
  }
  return tokens;
}

char Lexer::peek(int offset) { return source[pos + offset]; }
char Lexer::advance() {
  if (source[pos] == '\n') {
    loc.line++;
    loc.column = 0;
  } else {
    loc.column++;
  }
  return source[pos++];
}
bool Lexer::isAtEnd() { return pos >= source.size(); }

void Lexer::skipWhitespace() {
  while (isspace(peek())) {
    advance();
  }
}

std::optional<Token> Lexer::nextToken() {
  if (isAtEnd())
    return std::nullopt; // EOFのとき
  char c = peek();

  if (isalpha(c) || c == '_')
    return parseWords();
  if (isdigit(c))
    return parseNumber();

  return parseSymbols();
}

inline static std::map<std::string, TokenType> KEYWORDS = {
    {"fn", TokenType::FN},
    {"return", TokenType::RETURN},
    {"let", TokenType::LET},
};

Token Lexer::parseWords() {
  std::string value;
  while (!isAtEnd() && (std::isalnum(peek()) || peek() == '_')) {
    value += advance();
  }
  if (KEYWORDS.contains(value)) {
    return makeToken(KEYWORDS[value], value);
  }
  return makeToken(TokenType::IDENTIFIER, value);
}
Token Lexer::parseNumber() {
  std::string value;
  while (!isAtEnd() && isdigit(peek())) {
    value += advance();
  }
  return makeToken(TokenType::NUMBER, value);
}
Token Lexer::parseSymbols() {
  char c = advance();
  switch (c) {
  case ';':
    return makeToken(TokenType::SEMI, ";");
  case '(':
    return makeToken(TokenType::LPAREN, "(");
  case ')':
    return makeToken(TokenType::RPAREN, ")");
  case '{':
    return makeToken(TokenType::LBRACKET, "{");
  case '}':
    return makeToken(TokenType::RBRACKET, "}");
  default:
    error::ErrorHandler::getInstance().report(
        error::Code::ERR_LEX_UNKNOWN_SYMBOL,
        {std::string(1, c), std::to_string(loc.line),
         std::to_string(loc.column)});
  }
  return makeToken(TokenType::UNKNOWN, std::string(1, c));
}
Token Lexer::makeToken(TokenType type, std::string value) {
  return Token{type, value, loc};
}

// For debug
inline static std::map<TokenType, std::string> tokenTypeToName = {
    {TokenType::FN, "FN"},
    {TokenType::RETURN, "RETURN"},
    {TokenType::LET, "LET"},
    {TokenType::IDENTIFIER, "IDENTIFIER"},
    {TokenType::NUMBER, "NUMBER"},
    {TokenType::SEMI, "SEMI"},
    {TokenType::LPAREN, "LPAREN"},
    {TokenType::RPAREN, "RPAREN"},
    {TokenType::LBRACKET, "LBRACKET"},
    {TokenType::RBRACKET, "RBRACKET"},
    {TokenType::UNKNOWN, "UNKNOWN"},
};
void Lexer::printTokens() {
  pos = 0;
  loc.line = 0;
  loc.column = 0;
  auto result = tokenize();
  std::cerr << "-----Tokenize Result-----  Tokens:" << result.size()
            << std::endl;
  for (auto &&token : result) {
    std::cerr << "{Token: " << tokenTypeToName[token.type]
              << " Value: " << token.value << " PLACE: " << token.loc.line
              << " " << token.loc.column << "}" << std::endl;
  }
}

} // namespace ally
