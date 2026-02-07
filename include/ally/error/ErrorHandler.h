#pragma once
#include "ally/error/ErrorCode.h"
#include <string>
#include <vector>

namespace ally {
namespace error {
struct Error {
  Code code;
  std::string message;
};

class ErrorHandler {
private:
  ErrorHandler() {}
  ~ErrorHandler() {}
  std::vector<Error> errors;
  static void printError(Error error);

public:
  static ErrorHandler &getInstance() {
    static ErrorHandler instance;
    return instance;
  }
  ErrorHandler(const ErrorHandler &) = delete;
  ErrorHandler &operator=(const ErrorHandler &) = delete;
  void report(Code code, std::vector<std::string> args); // エラー報告
  void printAllErrors();                                 // エラー出力
};
} // namespace error
} // namespace ally
