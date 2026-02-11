#include "ally/parser/parser.h"
#include "ally/ast/function.h"
#include "ally/ast/other.h"
#include "ally/ast/stmt.h"
#include "ally/error/ErrorHandler.h"
#include <iostream>

namespace ally {
std::vector<std::unique_ptr<ast::FunctionNode>> Parser::parse() {
  pos = 0;
  std::vector<std::unique_ptr<ast::FunctionNode>> result;
  while (!isAtEnd()) {
    result.push_back(nextFunction());
  }
  return result;
}

const Token &Parser::peek(int offset) const { return source[pos + offset]; }
const Token &Parser::advance() {
  loc = source[pos].loc;
  return source[pos++];
}
bool Parser::isAtEnd() const { return pos >= source.size(); }
bool Parser::check(TokenType type) const { return peek().type == type; }
bool Parser::match(TokenType type) {
  if (check(type)) {
    advance();
    return true;
  }
  return false;
}

std::unique_ptr<ast::FunctionNode> Parser::nextFunction() {
  Location fnLocation = loc;
  if (advance().type != TokenType::FN) {
    std::cerr << "in parsing functionNode, advance().type != TokenType::FN!"
              << std::endl;
    return nullptr;
  }
  std::string funcName = advance().value;
  if (!match(TokenType::LPAREN)) {
    // Expected '(' after function name.
  }
  // TODO: 引数
  if (!match(TokenType::RPAREN)) {
    // Expected ')' after args.
  }
  auto block = parseBlock();
  return std::make_unique<ast::FunctionNode>(funcName, std::vector<ast::Arg>{},
                                             std::move(block), fnLocation);
}

std::unique_ptr<ast::ExprNode> Parser::parseExpr() {
  if (peek().type == TokenType::NUMBER)
    return parseNumberLiteral();
  else {
    error::ErrorHandler::getInstance().report(
        error::Code::ERR_PAR_UNKNOWN_EXPR_TOKEN,
        {std::to_string(loc.line), std::to_string(loc.column)});
    return nullptr;
  }
}

std::unique_ptr<ast::StmtNode> Parser::parseStmt() {
  if (peek().type == TokenType::LBRACKET)
    return parseBlock();
  else if (peek().type == TokenType::RETURN)
    return parseReturnStmt();
  else if (peek().type == TokenType::LET)
    return parseLetStmt();
  else {
    error::ErrorHandler::getInstance().report(
        error::Code::ERR_PAR_UNKNOWN_STMT_TOKEN,
        {std::to_string(loc.line), std::to_string(loc.column)});
    return nullptr;
  }
}

std::unique_ptr<ast::NumberLiteralNode> Parser::parseNumberLiteral() {
  Location nowLoc = loc;
  std::string value = advance().value;
  return std::make_unique<ast::NumberLiteralNode>(std::stol(value), nowLoc);
}

std::unique_ptr<ast::ReturnNode> Parser::parseReturnStmt() {
  Location nowLoc = loc;
  if (!match(TokenType::RETURN)) {
    // TODO: エラー
  }
  auto expr = parseExpr();
  return std::make_unique<ast::ReturnNode>(std::move(expr), nowLoc);
}

std::unique_ptr<ast::BlockNode> Parser::parseBlock() {
  Location nowLoc = loc;
  if (!match(TokenType::LBRACKET)) {
    // TODO: エラー
  }
  std::vector<std::unique_ptr<ast::StmtNode>> stmt;
  while (!check(TokenType::RBRACKET)) {
    stmt.push_back(parseStmt());
  }
  advance(); // RBRACKET用
  return std::make_unique<ast::BlockNode>(std::move(stmt), nowLoc);
}

std::unique_ptr<ast::LetNode> Parser::parseLetStmt() {
  Location nowLoc = loc;
  if (!match(TokenType::LET)) {
    // TODO: エラー
  }
  if (!check(TokenType::IDENTIFIER)) {
    // TODO: エラー(変数名のところに予約語をおくな)
  }
  std::string varName = advance().value;
  std::unique_ptr<ast::ExprNode> expr = nullptr;
  if (match(TokenType::EQ)) {
    expr = parseExpr();
  }
  return std::make_unique<ast::LetNode>(varName, false,
                                        ast::Type(ast::TypeInfo::INT, 0),
                                        std::move(expr), nowLoc);
}

} // namespace ally
