#pragma once
#include "other.h"
#include "stmt.h"

namespace ally::ast {

struct Arg {
  Type type;
  std::string name;
  void dump(int indent);
};
class BlockNode;
class FunctionNode : public Node {
  std::vector<Arg> args;
  std::string functionName;
  std::unique_ptr<BlockNode> block;

public:
  FunctionNode(std::string funcName, std::vector<Arg> arg,
               std::unique_ptr<BlockNode> block, Location loc)
      : Node(NodeType::FUNCTION, loc), functionName(std::move(funcName)),
        args(std::move(arg)), block(std::move(block)) {}
  void firstAnalysis();
  void secondAnalysis();
  Node *analysis() override;
  std::vector<Arg> getArgs() { return args; }
  std::string getFunctionName() { return functionName; }
  std::unique_ptr<BlockNode> getBlock() { return std::move(block); }
  void dump(int indent) override;
};

}; // namespace ally::ast
