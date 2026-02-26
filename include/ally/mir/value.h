#pragma once
#include "ally/ast/other.h"
#include <string>
#include <variant>

namespace ally::mir {
class Value {
  ast::Type type;
  std::string name;

public:
  Value(ast::Type t, std::string n) : type(t), name(n) {}
  virtual ~Value() = default;
  ast::Type getType() const { return type; }
  virtual void dump(int indent) = 0;
};

class ConstantValue : public Value {
  using ValueType = std::variant<int64_t, double, bool, std::string>;
  ValueType value;

public:
  ConstantValue(ast::Type t, int64_t val) : Value(t, "const") { value = val; }
  ConstantValue(ast::Type t, double val) : Value(t, "const") { value = val; }
  ConstantValue(ast::Type t, bool val) : Value(t, "const") { value = val; }
  ConstantValue(ast::Type t, std::string val) : Value(t, "const") {
    value = std::move(val);
  }
  void dump(int indent) override;
  ValueType getValue() { return value; }
};

class VariableValue : public Value {
  std::string varName;

public:
  VariableValue(ast::Type t, std::string name)
      : Value(t, name), varName(std::move(name)) {}
  void dump(int indent) override;
  std::string getVarName() { return varName; }
};
} // namespace ally::mir
