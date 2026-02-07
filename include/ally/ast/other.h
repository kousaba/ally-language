#pragma once
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
};
class Node {
  NodeType nodeType;
  int line;
  int column;

public:
  Node(NodeType type, int line, int column)
      : nodeType(type), line(line), column(column) {}
  virtual ~Node() = default;
};
struct Arg {
  Type type;
  std::string name;
};
class FunctionNode : public Node {
  std::vector<Arg> args;
  std::string functionName;

public:
  FunctionNode(std::string funcName, std::vector<Arg> arg, int line, int column)
      : functionName(std::move(funcName)), args(std::move(arg)),
        Node(NodeType::FUNCTION, line, column) {}
};
} // namespace ally::ast
