#include "ally/error/ErrorCode.h"
#include "ally/error/ErrorHandler.h"
#include "ally/error/ErrorMessage.h"
#include <iostream>

int main(int argc, char *argv[]) {
  std::cout << "This is ally compiler!" << std::endl;
  ally::error::ErrorMessageManager::getInstance().setLanguage(
      ally::error::Language::ja);
  ally::error::ErrorHandler::getInstance().report(
      ally::error::Code::COM_UNKNOWN_VAR, {});
  ally::error::ErrorHandler::getInstance().printAllErrors();
  return 0;
}
