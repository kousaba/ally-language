#pragma once
#include "ally/ast/other.h"
#include <string>
#include <variant>
#include <vector>

namespace ally::sema {
enum class SymbolType { VAR, UNKNOWN, FUNCTION };
struct Arg {
  ast::Type type;
  std::string name;
};
struct FunctionSymbol {
  std::vector<Arg> args;
  ast::Type retType;
};
struct VarSymbol {
  ast::Type varType;
  bool isMutable;
};
struct UnknownSymbol {};
struct Symbol {
  SymbolType type;
  std::variant<FunctionSymbol, VarSymbol, UnknownSymbol> symbol;
  Symbol(FunctionSymbol symbol) : symbol(symbol), type(SymbolType::FUNCTION) {}
  Symbol(VarSymbol symbol) : symbol(symbol), type(SymbolType::VAR) {}
  Symbol(UnknownSymbol symbol) : symbol(symbol), type(SymbolType::UNKNOWN) {}
  Symbol() : symbol(UnknownSymbol{}), type(SymbolType::UNKNOWN) {}
  explicit operator bool() const { return type != SymbolType::UNKNOWN; }
};
} // namespace ally::sema
