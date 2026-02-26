#include "ally/mir/builder.h"
#include "ally/ast/expr.h"
#include "ally/ast/function.h"
#include "ally/ast/other.h"
#include "ally/mir/node/expr.h"
#include <string>
#include <vector>

namespace ally::mir {
void MIRBuilder::addScope() {
  scopes.push_back(std::make_unique<SymbolTable>(topScope()));
}
void MIRBuilder::removeScope() {
  if (scopes.size() <= 1) {
    // Compiler Error:
    // スコープに入っていない状態でスコープを抜けようとしている(グローバルスコープは残す)
    return;
  }
  scopes.pop_back();
}
SymbolTable *MIRBuilder::topScope() { return scopes.back().get(); }
std::string MIRBuilder::getNextConstantName() {
  return "_A_Constant_" + std::to_string(constantCount++);
}
std::string MIRBuilder::getNextVariableName(std::string varName) {
  return "_A_Var_" + varName + std::to_string(++varNameCount[varName]);
}
std::string MIRBuilder::getNowVariableName(std::string varName) {
  return "_A_Var_" + varName + std::to_string(varNameCount[varName]);
}
Function *MIRBuilder::createFunction(std::string name, ast::Type retType,
                                     std::vector<Arg> arg) {
  mir.push_back(std::make_unique<Function>(name, retType, arg));
  return mir.back().get();
}
Block *MIRBuilder::createBlock(std::string name) {
  return currentFunction->createBlock(name +
                                      std::to_string(blockCount[name]++));
}
void MIRBuilder::setFunction(Function *f) { currentFunction = f; }
void MIRBuilder::insert(std::unique_ptr<StmtNode> stmt) {
  currentBlock->addStatement(std::move(stmt));
}
void MIRBuilder::insert(std::vector<std::unique_ptr<StmtNode>> stmts) {
  currentBlock->addStatement(std::move(stmts));
}
bool MIRBuilder::terminate(std::unique_ptr<Terminator> term) {
  return currentBlock->setTerminator(std::move(term));
}
void MIRBuilder::setBlock(Block *b) { currentBlock = b; }
void MIRBuilder::buildNextFunction(std::unique_ptr<ast::FunctionNode> func) {
  setFunction(createFunction(func->getFunctionName(),
                             ast::Type(ast::TypeInfo::INT), {}));
  setBlock(createBlock("entry"));
  buildStmt(func->getBlock().get());
}
std::vector<std::unique_ptr<Function>> MIRBuilder::build() {
  for (auto &&func : functions) {
    std::cout << "Building function.." << std::endl;
    std::cout << func->getFunctionName() << std::endl;
    buildNextFunction(std::move(func));
  }
  return std::move(mir);
}
std::unique_ptr<ExprNode> MIRBuilder::buildExpr(ast::ExprNode *expr) {
  if (!expr) {
    // TODO: Compiler Error: nullptrをbuildしようとした
  }
  if (auto numLiteral = dynamic_cast<ast::NumberLiteralNode *>(expr))
    return buildNumberLiteralExpr(numLiteral);
  else if (auto binaryop = dynamic_cast<ast::BinaryOpNode *>(expr))
    return buildBinaryOp(binaryop);
  else if (auto var = dynamic_cast<ast::VariableRefNode *>(expr))
    return buildVariableRef(var);
  else {
    // TODO: Compiler Error: 不明なexpr
  }
  return nullptr;
}
std::unique_ptr<ExprNode>
MIRBuilder::buildNumberLiteralExpr(ast::NumberLiteralNode *expr) {
  return std::make_unique<ValueNode>(std::make_unique<ConstantValue>(
      ast::Type(ast::TypeInfo::INT), expr->getNumber()));
}
std::unique_ptr<ExprNode> MIRBuilder::buildBinaryOp(ast::BinaryOpNode *expr) {
  // TODO
  std::unique_ptr<ExprNode> lhs = buildExpr(expr->getLhs().get());
  std::unique_ptr<ExprNode> rhs = buildExpr(expr->getRhs().get());
  std::string opStr = expr->getOp();
  BinaryOpNode::Op op;
  if (opStr == "+")
    op = BinaryOpNode::Op::ADD;
  else if (opStr == "-")
    op = BinaryOpNode::Op::SUB;
  else if (opStr == "*")
    op = BinaryOpNode::Op::MUL;
  else if (opStr == "/")
    op = BinaryOpNode::Op::DIV;
  else {
    // TODO: Compiler Error: 不明なopStr
  }
  return std::make_unique<BinaryOpNode>(std::move(lhs), std::move(rhs), op);
}
std::unique_ptr<ExprNode>
MIRBuilder::buildVariableRef(ast::VariableRefNode *expr) {
  return std::make_unique<ValueNode>(std::make_unique<VariableValue>(
      ast::Type(expr->getType()), getNowVariableName(expr->getVarName())));
}
void MIRBuilder::buildStmt(ast::StmtNode *stmt) {
  if (!stmt) {
    // TODO: Compiler Error: nullptrをbuildしようとした
  }
  if (auto *ret = dynamic_cast<ast::ReturnNode *>(stmt))
    buildReturnStmt(ret);
  else if (auto *let = dynamic_cast<ast::LetNode *>(stmt))
    buildLetStmt(let);
  else if (auto *block = dynamic_cast<ast::BlockNode *>(stmt))
    buildBlock(block);
  else {
    // TODO: Compiler Error: 不明なstmt
  }
}
void MIRBuilder::buildReturnStmt(ast::ReturnNode *stmt) {
  terminate(std::move(
      std::make_unique<ReturnTerminator>(buildExpr(stmt->getValue().get()))));
}
void MIRBuilder::buildLetStmt(ast::LetNode *stmt) {
  std::string newVarName = getNextVariableName(stmt->getVarName());
  ast::Type varType = stmt->getVarType();
  std::unique_ptr<ast::ExprNode> expr = stmt->getInitExpr();
  std::unique_ptr<ExprNode> val = buildExpr(expr.get());
  if (!expr)
    std::cout << "Expr is null" << std::endl;
  if (!val)
    std::cout << "Val is null" << std::endl;
  insert(std::make_unique<AssignmentNode>(newVarName, varType, std::move(val)));
}
void MIRBuilder::buildBlock(ast::BlockNode *block) {
  addScope();
  for (auto &&stmt : block->getStatements()) {
    buildStmt(stmt.get());
  }
  removeScope();
}
} // namespace ally::mir
