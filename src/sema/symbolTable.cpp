#include "ally/sema/SymbolTable.h"
#include <iostream>

namespace ally::sema {
Symbol SymbolTable::lookupSymbol(std::string name) const {
  for (auto &&mp : symbols) {
    if (mp.contains(name)) {
      return mp.find(name)->second;
    }
  }
  return Symbol(UnknownSymbol());
}
bool SymbolTable::addSymbol(std::string name, Symbol symbol) {
  // TODO: ErrorHandlerを使う
  if (symbols.empty()) {
    std::cerr << "Error: No scope available!" << std::endl;
    return false;
  }
  if (lookupSymbol(name).type != SymbolType::UNKNOWN) {
    return false;
  }
  symbols.back()[name] = symbol;
  return true;
}
void SymbolTable::addScope() {
  symbols.push_back(std::map<std::string, Symbol>{});
}
void SymbolTable::removeScope() { symbols.pop_back(); }
void SymbolTable::dump() {
  std::cout << "--- SymbolTable Dump ---" << std::endl;
  for (const auto &scope : symbols) {
    for (const auto &[name, symbol] : scope) {
      std::cout << "Name: " << name;
      if (symbol.type == SymbolType::FUNCTION) {
        std::cout << " [Function]";
      }
      std::cout << std::endl;
    }
  }
}
} // namespace ally::sema
