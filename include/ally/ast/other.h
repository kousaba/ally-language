#pragma once
#include "ally/location.h"
#include <memory>
#include <string>
#include <vector>
namespace ally::ast {
enum class TypeInfo { INT, I32, I16, FLOAT, F32, F16 };

struct Type {
  TypeInfo info;
  int pointer = 0; // *なら1, **なら2
};
enum class NodeType {
  TYPE,
  FUNCTION,
  NUMBERLITERAL,
  RETURN,
  BLOCK,
};
class Node {
  NodeType nodeType;
  Location loc;

public:
  Node(NodeType type, Location loc) : nodeType(type), loc(loc) {}
  virtual ~Node() = default;
};
} // namespace ally::ast
