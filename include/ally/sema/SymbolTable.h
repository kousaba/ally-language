#pragma once
#include "Symbol.h"
#include <map>
#include <memory>
#include <vector>

namespace ally::sema {
class SymbolTable {
private:
  std::vector<std::map<std::string, Symbol>> symbols;
  std::vector<FunctionSymbol> functions;
  std::unique_ptr<FunctionSymbol> lookingFunctionSymbol;
  SymbolTable() = default;
  ~SymbolTable() = default;

public:
  Symbol lookupSymbol(std::string name) const;
  bool addSymbol(std::string name,
                 Symbol symbol); // 成功したらtrue, 失敗したらfalse
  void addScope();
  void removeScope();
  SymbolTable(const SymbolTable &) = delete;
  SymbolTable &operator=(const SymbolTable &) = delete;
  SymbolTable(SymbolTable &&) = delete;
  SymbolTable &operator=(SymbolTable &&) = delete;
  static SymbolTable &getInstance() {
    static SymbolTable instance;
    return instance;
  }
  void dump();
};
} // namespace ally::sema
