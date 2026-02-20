#pragma once

namespace ally {
struct Location {
  int line;
  int column;
  Location() : line(0), column(0) {}
  Location(int line, int column) : line(line), column(column) {}
};
} // namespace ally
