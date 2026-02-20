#pragma once
#include <iostream>

namespace ally::mir {
enum class NodeType { UNKNOWN, ASSIGNMENT };
class Node {
private:
  NodeType type;

public:
  Node(NodeType type) : type(type) {}
  virtual ~Node() = default;
  NodeType getType();
  virtual bool isExpr();
  virtual bool isStmt();
  virtual void dump(int indent) = 0;
};

void printIndent(int indent);

class UnknownNode : public Node {
public:
  UnknownNode() : Node(NodeType::UNKNOWN) {}
  void dump(int indent) override;
};
} // namespace ally::mir
