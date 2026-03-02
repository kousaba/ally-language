#pragma once
#include "ally/ast/expr.h"
#include "ally/ast/function.h"
#include "ally/ast/stmt.h"
#include "function.h"
#include "symboltable.h"
#include <map>
#include <vector>

namespace ally::mir {
class MIRBuilder {
  Function *currentFunction;
  Block *currentBlock;
  int constantCount;
  std::map<std::string, int> varNameCount;
  std::vector<std::unique_ptr<ast::FunctionNode>> functions;
  std::map<std::string, int> blockCount;
  std::vector<std::unique_ptr<SymbolTable>> scopes;
  std::vector<std::unique_ptr<Function>> mir;
  std::vector<std::unique_ptr<AllocaNode>> allocaNodes;

private:
  void addScope();
  void removeScope();
  SymbolTable *topScope();
  std::string getNextConstantName();
  std::string getNextVariableName(std::string varName);
  std::string getNowVariableName(std::string varName);
  Function *createFunction(std::string name, ast::Type type,
                           std::vector<Arg> args);
  Block *createBlock(std::string name);
  void resetCounter();
  void setFunction(Function *f);
  void insert(std::unique_ptr<StmtNode> stmt);
  void insert(std::vector<std::unique_ptr<StmtNode>> stmts);
  void insertAlloca(std::unique_ptr<AllocaNode> alloca);
  void insertAlloca(std::string name, ast::Type t);
  bool terminate(std::unique_ptr<Terminator> term);
  void setBlock(Block *b);
  void buildNextFunction(std::unique_ptr<ast::FunctionNode> func);
  std::unique_ptr<ExprNode> buildExpr(ast::ExprNode *expr);
  std::unique_ptr<ExprNode>
  buildNumberLiteralExpr(ast::NumberLiteralNode *expr);
  std::unique_ptr<ExprNode> buildBinaryOp(ast::BinaryOpNode *expr);
  std::unique_ptr<ExprNode> buildVariableRef(ast::VariableRefNode *expr);
  void buildStmt(ast::StmtNode *stmt);
  void buildReturnStmt(ast::ReturnNode *stmt);
  void buildLetStmt(ast::LetNode *stmt);
  void buildBlock(ast::BlockNode *block);
  void buildIfStmt(ast::IfNode *stmt);

public:
  MIRBuilder(std::vector<std::unique_ptr<ast::FunctionNode>> funcs)
      : constantCount(0), functions(std::move(funcs)) {
    scopes.push_back(std::make_unique<SymbolTable>(nullptr));
  }
  std::vector<std::unique_ptr<Function>> build();
};
} // namespace ally::mir
