#pragma once
namespace ally {
namespace error {
// ErrorTypes:
// FAT -> Fatal Error
// COM -> Compiler Error(Debug)
// ERR -> Error
// WAR -> Warn
// INF -> INFO
// ErrorPlace
// LEX -> Lexer
// PAR -> Parser
// SEM -> Semantic Analysis
// COD -> CodeGen
// ErrorType_ErrorPlace_Error
enum class Code {
  ERR_LEX_UNKNOWN_SYMBOL,
  ERR_PAR_UNKNOWN_EXPR_TOKEN,
  ERR_PAR_UNKNOWN_STMT_TOKEN,
  ERR_SEM_MULTIPLE_FUNCTION_DECL,
  COM_SEM_FUNCTIONNODE_VISITED,
  COM_SEM_EXPECTED_EXPR,
  ERR_SEM_RETURN_UNEXPECTED_TYPE,
};
} // namespace error
} // namespace ally
