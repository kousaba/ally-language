#pragma once
#include "base.h"

namespace ally::mir {
class ExprNode : public Node {
public:
  using Node::Node;
  bool isExpr() override;
};
} // namespace ally::mir
