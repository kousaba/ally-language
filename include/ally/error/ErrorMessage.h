#pragma once
#include "ErrorCode.h"
#include <map>
#include <optional>
#include <string>

namespace ally {
namespace error {
struct Message {
  std::string en;
  std::string ja;
};
enum class Language { en, ja };
class ErrorMessageManager {
private:
  ErrorMessageManager();
  ~ErrorMessageManager() {}
  void addMessage(Code code, Message message);
  std::map<Code, Message> messages;
  Language language;

public:
  static ErrorMessageManager &getInstance() {
    static ErrorMessageManager instance;
    return instance;
  }
  ErrorMessageManager(const ErrorMessageManager &) = delete;
  ErrorMessageManager &operator=(const ErrorMessageManager &) = delete;
  // エラーメッセージ取得
  std::optional<std::string> getErrorMessage(Code code);
  // 言語設定
  void setLanguage(Language lang) { this->language = lang; }
  Language getLanguage() { return language; }
};
} // namespace error
} // namespace ally
