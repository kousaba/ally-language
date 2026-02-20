#pragma once
#include "value.h"
#include <map>
#include <string>

namespace ally::mir {
class SymbolTable {
  std::map<std::string, Value *> table;
  SymbolTable *parent; // スコープ管理はparentで行う
public:
  SymbolTable(SymbolTable *p = nullptr) : parent(p) {}
  void define(const std::string &name, Value *val) { table[name] = val; }
  Value *lookupCurrentScope(const std::string &name) {
    if (table.contains(name)) {
      return table[name];
    }
    return nullptr;
  }
  Value *lookup(const std::string &name) {
    if (auto res = lookupCurrentScope(name))
      return res;
    if (parent)
      return parent->lookup(name);
    return nullptr;
  }
};
} // namespace ally::mir
