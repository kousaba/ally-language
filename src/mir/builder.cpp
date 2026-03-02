#include "ally/mir/builder.h"
#include "ally/ast/expr.h"
#include "ally/ast/function.h"
#include "ally/ast/other.h"
#include "ally/error/ErrorCode.h"
#include "ally/error/ErrorHandler.h"
#include "ally/mir/node/expr.h"
#include "ally/mir/terminator.h"
#include <string>
#include <vector>

namespace ally::mir {
void MIRBuilder::addScope() {
  scopes.push_back(std::make_unique<SymbolTable>(topScope()));
}
void MIRBuilder::removeScope() {
  if (scopes.size() <= 1) {
    error::ErrorHandler::getInstance().report(
        error::Code::COM_MIR_NO_SCOPE_AVAILABLE, {});
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
void MIRBuilder::setFunction(Function *f) {
  if (!f)
    return;
  if (currentFunction) {
    currentFunction->getEntryBlock()->insertAllocaNodes(std::move(allocaNodes));
    allocaNodes = std::vector<std::unique_ptr<AllocaNode>>{};
  }
  currentFunction = f;
}
void MIRBuilder::insert(std::unique_ptr<StmtNode> stmt) {
  currentBlock->addStatement(std::move(stmt));
}
void MIRBuilder::insert(std::vector<std::unique_ptr<StmtNode>> stmts) {
  currentBlock->addStatement(std::move(stmts));
}
void MIRBuilder::insertAlloca(std::unique_ptr<AllocaNode> alloca) {
  allocaNodes.push_back(std::move(alloca));
}
void MIRBuilder::insertAlloca(std::string name, ast::Type t) {
  insertAlloca(std::make_unique<AllocaNode>(name, t));
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
  setFunction(nullptr);
  return std::move(mir);
}
std::unique_ptr<ExprNode> MIRBuilder::buildExpr(ast::ExprNode *expr) {
  if (!expr) {
    error::ErrorHandler::getInstance().report(error::Code::COM_MIR_EXPR_IS_NULL,
                                              {});
  }
  if (auto numLiteral = dynamic_cast<ast::NumberLiteralNode *>(expr))
    return buildNumberLiteralExpr(numLiteral);
  else if (auto binaryop = dynamic_cast<ast::BinaryOpNode *>(expr))
    return buildBinaryOp(binaryop);
  else if (auto var = dynamic_cast<ast::VariableRefNode *>(expr))
    return buildVariableRef(var);
  else {
    error::ErrorHandler::getInstance().report(error::Code::COM_MIR_UNKNOWN_EXPR,
                                              {});
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
  else if (opStr == "==")
    op = BinaryOpNode::Op::EQEQ;
  else if (opStr == "!=")
    op = BinaryOpNode::Op::NEQ;
  else if (opStr == "<")
    op = BinaryOpNode::Op::LT;
  else if (opStr == "<=")
    op = BinaryOpNode::Op::LTE;
  else if (opStr == ">")
    op = BinaryOpNode::Op::GT;
  else if (opStr == ">=")
    op = BinaryOpNode::Op::LTE;
  else {
    error::ErrorHandler::getInstance().report(
        error::Code::COM_MIR_UNKNOWN_OPSTR, {opStr});
    // TODO: Compiler Error: 不明なopStr
  }
  return std::make_unique<BinaryOpNode>(std::move(lhs), std::move(rhs), op);
}
std::unique_ptr<ExprNode>
MIRBuilder::buildVariableRef(ast::VariableRefNode *expr) {
  return std::make_unique<LoadNode>(expr->getVarName(), expr->getType());
}
void MIRBuilder::buildStmt(ast::StmtNode *stmt) {
  if (!stmt) {
    error::ErrorHandler::getInstance().report(error::Code::COM_MIR_STMT_IS_NULL,
                                              {});
  }
  if (auto *ret = dynamic_cast<ast::ReturnNode *>(stmt))
    buildReturnStmt(ret);
  else if (auto *let = dynamic_cast<ast::LetNode *>(stmt))
    buildLetStmt(let);
  else if (auto *block = dynamic_cast<ast::BlockNode *>(stmt))
    buildBlock(block);
  else if (auto *ifStmt = dynamic_cast<ast::IfNode *>(stmt))
    buildIfStmt(ifStmt);
  else {
    error::ErrorHandler::getInstance().report(error::Code::COM_MIR_UNKNOWN_STMT,
                                              {});
  }
}
void MIRBuilder::buildReturnStmt(ast::ReturnNode *stmt) {
  terminate(std::move(
      std::make_unique<ReturnTerminator>(buildExpr(stmt->getValue().get()))));
}
void MIRBuilder::buildLetStmt(ast::LetNode *stmt) {
  std::string varName = stmt->getVarName();
  ast::Type varType = stmt->getVarType();
  insert(std::make_unique<AllocaNode>(varName, varType));
  std::unique_ptr<ExprNode> val = buildExpr(stmt->getInitExpr().get());
  insert(std::make_unique<StoreNode>(varName, std::move(val)));
}
void MIRBuilder::buildBlock(ast::BlockNode *block) {
  addScope();
  for (auto &&stmt : block->getStatements()) {
    buildStmt(stmt.get());
  }
  removeScope();
}
void MIRBuilder::buildIfStmt(ast::IfNode *stmt) {
  std::unique_ptr<ExprNode> cond = buildExpr(stmt->getCondition().get());
  Block *thenB = createBlock("if.then");
  Block *elseB = stmt->hasElse() ? createBlock("if.else") : nullptr;
  Block *mergeB = createBlock("if.merge");
  Block *falseBlock = elseB ? elseB : mergeB;
  terminate(std::make_unique<IfTerminator>(std::move(cond), thenB, falseBlock));
  // ThenBlockの構築
  setBlock(thenB);
  buildStmt(stmt->getThenBranch().get());
  // returnなどがなければmergeにジャンプ
  if (!currentBlock->hasTerminator()) {
    terminate(std::make_unique<JumpTerminator>(mergeB));
  }
  // ElseBlockの構築
  if (elseB) {
    setBlock(elseB);
    buildStmt(stmt->getElseBranch().get());
    if (!currentBlock->hasTerminator()) {
      terminate(std::make_unique<JumpTerminator>(mergeB));
    }
  }
  setBlock(mergeB);
}
} // namespace ally::mir
