#include "ally/error/ErrorHandler.h"
#include "ally/error/ErrorMessage.h"
#include <iostream>
#include <string>

namespace ally {
namespace error {
void ErrorHandler::printError(Error error) {
  // TODO: Error, Fatalなどを分ける
  std::cerr << "[Error]: " << error.message << std::endl;
}

void ErrorHandler::report(Code code, std::vector<std::string> args) {
  auto result = ErrorMessageManager::getInstance().getErrorMessage(code);
  std::string message =
      "Unknown Error Message! Please check code and language!";
  if (result) {
    message = *result;
  }
  for (int i = 0; i < 9 && i < args.size(); i++) {
    std::string target = "%" + std::to_string(i + 1);
    size_t pos = message.find(target);
    while (pos != std::string::npos) {
      message.replace(pos, target.length(), args[i]);
      pos = message.find(target, pos + args[i].length());
    }
  }
  errors.push_back(Error{code, message});
}

void ErrorHandler::printAllErrors() {
  for (auto &&err : errors) {
    printError(err);
  }
}

} // namespace error
} // namespace ally
