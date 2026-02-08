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
  addMessage(Code::ERR_LEX_UNKNOWN_SYMBOL,
             {"Unknown Symbol %1 Detected in (line: %2, column: %3)",
              "不明な記号%1が行%2、列%3で検知されました"});
  addMessage(Code::ERR_PAR_UNKNOWN_EXPR_TOKEN,
             {"Unknown Token Detected in expr: (line: %1, column: %2)",
              "不明なトークンが式: 行%1、列%2で検知されました"});
  addMessage(Code::ERR_PAR_UNKNOWN_STMT_TOKEN,
             {"Unknown Token Detected in statement: (line: %1, column: %2)",
              "不明なトークンが分: 行%1、列%2で検知されました"});
}
} // namespace error
} // namespace ally
