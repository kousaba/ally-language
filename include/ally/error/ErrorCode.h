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
};
} // namespace error
} // namespace ally
