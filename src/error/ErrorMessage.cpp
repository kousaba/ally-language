#include "ally/error/ErrorMessage.h"
#include "ally/error/ErrorCode.h"
#include <iostream>

namespace ally {
namespace error {
void ErrorMessageManager::addMessage(Code code, Message message) {
  messages[code] = message;
}
std::optional<std::string> ErrorMessageManager::getErrorMessage(Code code) {
  if (messages.contains(code)) {
    switch (language) {
    case Language::en:
      return messages[code].en;
    case Language::ja:
      return messages[code].ja;
    }
    return std::nullopt; // 言語が見つからなかった
  }
  return std::nullopt; // エラーメッセージが見つからなかった
}
ErrorMessageManager::ErrorMessageManager() {
  addMessage(Code::COM_UNKNOWN_VAR, {"HI!!", "This is ErrorMessage"});
}
} // namespace error
} // namespace ally
