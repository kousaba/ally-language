#pragma once
#include "node/expr.h"
#include "value.h"
#include <memory>

namespace ally::mir {
class Block;
enum class TerminatorType { RETURN, JUMP, IF, UNREACHABLE };
class Terminator {
protected:
  TerminatorType type;

public:
  TerminatorType getType() { return type; }
  Terminator(TerminatorType type) : type(type) {}
  virtual ~Terminator() = default;
  virtual void dump(int indent) = 0;
};

class ReturnTerminator : public Terminator {
  std::unique_ptr<Value> returnValue;

public:
  ReturnTerminator(std::unique_ptr<Value> retVal)
      : Terminator(TerminatorType::RETURN), returnValue(std::move(retVal)) {}
  void dump(int indent) override;
};

class JumpTerminator : public Terminator {
  Block *to;

public:
  JumpTerminator(Block *t) : Terminator(TerminatorType::JUMP), to(t) {}
  void dump(int indent) override;
};

class IfTerminator : public Terminator {
  std::unique_ptr<ExprNode> cond;
  Block *thenB;
  Block *elseB;

public:
  IfTerminator(std::unique_ptr<ExprNode> c, Block *tB, Block *eB)
      : Terminator(TerminatorType::IF), cond(std::move(c)), thenB(tB),
        elseB(eB) {}
  void dump(int indent) override;
};

class UnreachableTerminator : public Terminator {
public:
  UnreachableTerminator() : Terminator(TerminatorType::UNREACHABLE) {}
  void dump(int indent) override;
};
} // namespace ally::mir
