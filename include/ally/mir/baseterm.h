#pragma once
namespace ally::mir{

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
}
