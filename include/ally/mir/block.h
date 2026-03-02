#pragma once
#include "node/stmt.h"
#include "baseterm.h"
#include <memory>
#include <vector>

namespace ally::mir {
class Block {
  std::string blockName;
  std::vector<std::unique_ptr<StmtNode>> statements;
  std::unique_ptr<Terminator> terminator;

public:
  Block(std::string blockName, std::vector<std::unique_ptr<StmtNode>> stmts,
        std::unique_ptr<Terminator> terminator)
      : statements(std::move(stmts)), terminator(std::move(terminator)) {}
  Block(std::string blockName) : blockName(blockName) { terminator = nullptr; };
  bool setTerminator(std::unique_ptr<Terminator> term) {
    if (terminator) {
      // TODO: CompilerError: Terminatorの二重設定
      return false;
    }
    if (!term) {
      // TODO: CompilerError: Terminatorにnullptrを設定している
      return false;
    }
    terminator = std::move(term);
    return true;
  }
  void addStatement(std::unique_ptr<StmtNode> stmt) {
    statements.push_back(std::move(stmt));
  }
  void addStatement(std::vector<std::unique_ptr<StmtNode>> stmts) {
    for (auto &&stmt : stmts) {
      statements.push_back(std::move(stmt));
    }
  }
  bool hasTerminator() { return terminator != nullptr; }
  void replaceTerminator(std::unique_ptr<Terminator> term) {
    if (term) {
      // TODO: Compiler Error: Terminatorをnullptrに置き換えようとしている
    }
    terminator = std::move(term);
  }
  void insertAllocaNodes(std::vector<std::unique_ptr<AllocaNode>> allocas){
    for(auto&& i : allocas){
      statements.insert(statements.begin(), std::move(i));
    }
  }
  void dump(int indent);
  std::string getBlockName() { return blockName; }
  virtual ~Block() = default;
};
} // namespace ally::mir
