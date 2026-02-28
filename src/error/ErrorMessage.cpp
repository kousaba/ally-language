#include "ally/error/ErrorMessage.h"

namespace ally {
namespace error {
void ErrorMessageManager::addMessage(Code code, std::string en,
                                     std::string ja) {
  Message msg(en, ja);
  messages[code] = msg;
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
             "Unknown Symbol %1 Detected in (line: %2, column: %3)",
             "不明な記号%1が行%2、列%3で検知されました");
  addMessage(Code::ERR_PAR_UNKNOWN_EXPR_TOKEN,
             "Unknown Token Detected in expr: (line: %1, column: %2)",
             "不明なトークンが式: 行%1、列%2で検知されました");
  addMessage(Code::ERR_PAR_UNKNOWN_STMT_TOKEN,
             "Unknown Token Detected in statement: (line: %1, column: %2)",
             "不明なトークンが分: 行%1、列%2で検知されました");
  addMessage(Code::ERR_SEM_MULTIPLE_FUNCTION_DECL,
             "Function %1 is defined multiple times",
             "関数%1が複数回定義されています");
  addMessage(Code::COM_SEM_FUNCTIONNODE_VISITED,
             "FunctionNode visited during SemanticAnalysis",
             "SemanticAnalysis中にFunctionNodeをvisitしました");
  addMessage(Code::COM_SEM_EXPECTED_EXPR,
             "Expecting an ExprNode, but something else came.",
             "ExprNodeを期待しましたが、それ以外のものが来ました");
  addMessage(Code::ERR_PAR_EXCEPTED_LPAREN_AFTER_FUNC,
             "Excepted '(' after function name.", "関数名の後に'('が必要です");
  addMessage(Code::ERR_PAR_EXPECTED_RPAREN_AFTER_ARGS,
             "Expected '(' after function args.",
             "関数の引数の後に')'が必要です");
  addMessage(Code::ERR_PAR_EXPECTED_IDENTIFIER_VAR,
             "Expected identifier after let but a reserved word was found.",
             "letの後に変数名を期待していましたが、予約語が見つかりました");
  addMessage(Code::COM_SEM_NO_SCOPE_AVAILABLE, "No scope available.",
             "使用可能なスコープがありません");
  addMessage(Code::ERR_SEM_SYMBOL_IS_NOT_VAR,
             "Variable reference refers to non-variable symbol %1.",
             "変数参照で変数以外のシンボル %1 を参照しています");
  addMessage(Code::ERR_SEM_VAR_MULTIPLE_DEFINED,
             "The variable %1 is already defined.",
             "変数%1は既に定義されています");
  addMessage(
      Code::ERR_SEM_VAR_NOT_MATCH_VALUE_TYPE,
      "The type of variable %1 does not match the type of its initial value.",
      "変数%1の型と初期値の型が一致していません");
  addMessage(Code::COM_MIR_NO_SCOPE_AVAILABLE, "No scope available.",
             "使用可能なスコープがありません");
  addMessage(Code::COM_MIR_EXPR_IS_NULL, "Expr is null.", "式がnullです");
  addMessage(Code::COM_MIR_UNKNOWN_EXPR, "Unknown ExprNode came.",
             "不明な式のノードです");
  addMessage(Code::COM_MIR_UNKNOWN_OPSTR,
             "Unknown BinaryOp.opStr %1 encountered.",
             "不明なBinaryOp.opSTr %1 が来ました");
  addMessage(Code::COM_MIR_STMT_IS_NULL, "Stmt is null.", "文がnullです");
  addMessage(Code::COM_MIR_UNKNOWN_STMT, "Unknown StmtNode came.",
             "不明な文のノードです");
}
} // namespace error
} // namespace ally
