#pragma once
#include "ally/ast/other.h"
#include "block.h"
#include <memory>
#include <string>
#include <vector>

namespace ally::mir {
struct Arg {
  ast::Type type;
  std::string name;
};
class Function {
  std::string funcName;
  ast::Type returnType;
  std::vector<Arg> args;
  std::vector<std::unique_ptr<Block>> blocks;

public:
  Function(std::string name, ast::Type retType, std::vector<Arg> arg)
      : funcName(name), returnType(retType), args(std::move(arg)) {}
  Block *createBlock(std::string name) {
    blocks.push_back(std::make_unique<Block>(name));
    return blocks.back().get();
  }
  Block *getEntryBlock() const {
    if (blocks.empty()) {
      // TODO: Compiler Error: EntryBlockが存在しません
      return nullptr;
    }
    return blocks.front().get();
  }
  void dump();
};
} // namespace ally::mir
