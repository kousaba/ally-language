#pragma once
#include "ally/location.h"
#include <iostream>
namespace ally::ast {
enum class TypeInfo { INT, I32, I16, FLOAT, F32, F16, UNKNOWN };

struct Type {
  TypeInfo info;
  int pointer = 0; // *なら1, **なら2
  bool operator==(Type another);
  bool operator!=(Type another);
  Type(TypeInfo info, int pointer = 0) : info(info), pointer(pointer) {}
};
enum class NodeType {
  TYPE,
  FUNCTION,
  NUMBERLITERAL,
  BINARYOP,
  RETURN,
  BLOCK,
  LET,
  VARREF,
};
class Node {
  NodeType nodeType;
  Location loc;

public:
  Node(NodeType type, Location loc) : nodeType(type), loc(loc) {}
  virtual ~Node() = default;
  virtual void dump(int indent) = 0;
  virtual Node *analysis() = 0;
};

void printIndent(int indent);
} // namespace ally::ast
