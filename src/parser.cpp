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

bool Parser::isBinaryOp(Token token) {
  TokenType type = token.type;
  return type == TokenType::ADD || type == TokenType::SUB ||
         type == TokenType::MUL || type == TokenType::DIV;
}
Parser::BindingPower Parser::getBindingPower(Token token) {
  switch (token.type) {
  case TokenType::ADD:
  case TokenType::SUB:
    return {110, 111};
  case TokenType::MUL:
  case TokenType::DIV:
    return {120, 121};
  case TokenType::EQEQ:
  case TokenType::NEQ:
    return {200, 201};
  case TokenType::LT:
  case TokenType::LTE:
  case TokenType::GT:
  case TokenType::GTE:
    return {210, 211};
  default:
    return {0, 0};
  }
}
std::unique_ptr<ast::ExprNode> Parser::parseExpr(int min_bp) {
  auto lhs = parsePrimary();
  if (!lhs)
    return nullptr;
  while (true) {
    Token op_token = peek();
    if (op_token.type == TokenType::UNKNOWN ||
        op_token.type == TokenType::SEMI || op_token.type == TokenType::RPAREN)
      break;
    BindingPower bp = getBindingPower(op_token);
    if (bp.left < min_bp)
      break;
    advance();
    Location now_loc = op_token.loc;
    auto rhs = parseExpr(bp.right);
    if (!rhs) {
      return nullptr;
    }
    lhs = std::make_unique<ast::BinaryOpNode>(std::move(lhs), op_token.value,
                                              std::move(rhs), loc);
  }
  return lhs;
}
std::unique_ptr<ast::ExprNode> Parser::parsePrimary() {
  if (peek().type == TokenType::LPAREN) {
    advance();
    auto expr = parseExpr(0);
    if (!expr || peek().type != TokenType::RPAREN) {
      return nullptr;
    }
    advance();
    return expr;
  }
  if (peek().type == TokenType::NUMBER) {
    Location nowLoc = loc;
    std::string value = advance().value;
    return std::make_unique<ast::NumberLiteralNode>(std::stol(value), nowLoc);
  }
  if (peek().type == TokenType::IDENTIFIER) {
    Location nowLoc = loc;
    std::string name = advance().value;
    return std::make_unique<ast::VariableRefNode>(std::move(name), nowLoc);
  }
  return nullptr;
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

std::unique_ptr<ast::ReturnNode> Parser::parseReturnStmt() {
  Location nowLoc = loc;
  if (!match(TokenType::RETURN)) {
    // TODO: エラー
  }
  auto expr = parseExpr();
  match(TokenType::SEMI);
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
  match(TokenType::SEMI);
  return std::make_unique<ast::LetNode>(varName, false,
                                        ast::Type(ast::TypeInfo::INT, 0),
                                        std::move(expr), nowLoc);
}

} // namespace ally
