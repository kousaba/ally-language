#pragma once
#include "ally/ast/expr.h"
#include "ally/ast/function.h"
#include "ally/ast/stmt.h"
#include "ally/lexer/token.h"
#include "ally/location.h"
#include <memory>

namespace ally {
class Parser {
private:
  std::vector<Token> source;
  size_t pos;
  Location loc;

public:
  Parser(std::vector<Token> tokens) : source(tokens), loc() {}
  std::vector<std::unique_ptr<ast::FunctionNode>> parse();

private:
  const Token &peek(int offset = 0) const;
  const Token &advance();
  bool isAtEnd() const;
  bool check(TokenType type) const;
  bool match(TokenType type);

  std::unique_ptr<ast::FunctionNode> nextFunction();
  std::unique_ptr<ast::ExprNode> parseExpr();
  std::unique_ptr<ast::StmtNode> parseStmt();
  // Expr
  std::unique_ptr<ast::NumberLiteralNode> parseNumberLiteral();
  // Stmt
  std::unique_ptr<ast::ReturnNode> parseReturnStmt();
  std::unique_ptr<ast::BlockNode> parseBlock();
  std::unique_ptr<ast::LetNode> parseLetStmt();
};
} // namespace ally
