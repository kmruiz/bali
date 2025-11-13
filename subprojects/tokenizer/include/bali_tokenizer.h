/**
 * @file
 * @brief Tokenizer for Bali. Operates on an in-memory C string. 
 * Does not allocate; all state lives in bali_lexer_t.
 */
#pragma once

#include <bali_utilities.h>
#include <stddef.h>
#include <stdbool.h>

/**
 * @brief Position of a character in the source.
 * index is 0-based; line and column are 1-based.
 */
typedef struct bali_position_t {
  /** 0-based character offset. **/
  bsize_t index;
  /** 1-based line number. **/
  bsize_t line;
  /** 1-based column number. **/
  bsize_t column;
} bali_position_t;

/**
 * @brief Half-open slice of the source [start, end).
 */
typedef struct bali_span_t {
  bali_position_t start;
  bali_position_t end;
} bali_span_t;

/**
 * @brief JavaScript token kinds recognized by the tokenizer.
 */
typedef enum bali_token_kind_t {
  TK_EOF,
  TK_NUMERIC_LITERAL_START,
  TK_NUMERIC_LITERAL_BIGINT = TK_NUMERIC_LITERAL_START,
  TK_NUMERIC_LITERAL_RATIONAL,
  TK_NUMERIC_LITERAL_END = TK_NUMERIC_LITERAL_RATIONAL,
  TK_KEYWORD_START,
  TK_KEYWORD_AWAIT = TK_KEYWORD_START,
  TK_KEYWORD_BREAK,
  TK_KEYWORD_CASE,
  TK_KEYWORD_CATCH,
  TK_KEYWORD_CLASS,
  TK_KEYWORD_CONST,
  TK_KEYWORD_CONTINUE,
  TK_KEYWORD_CONSTRUCTOR,
  TK_KEYWORD_DEBUGGER,
  TK_KEYWORD_DEFAULT,
  TK_KEYWORD_DELETE,
  TK_KEYWORD_DO,
  TK_KEYWORD_ELSE,
  TK_KEYWORD_EXPORT,
  TK_KEYWORD_EXTENDS,
  TK_KEYWORD_FALSE,
  TK_KEYWORD_FINALLY,
  TK_KEYWORD_FOR,
  TK_KEYWORD_FUNCTION,
  TK_KEYWORD_IF,
  TK_KEYWORD_IMPORT,
  TK_KEYWORD_IN,
  TK_KEYWORD_INSTANCEOF,
  TK_KEYWORD_LET,
  TK_KEYWORD_NEW,
  TK_KEYWORD_NULL,
  TK_KEYWORD_RETURN,
  TK_KEYWORD_STATIC,
  TK_KEYWORD_SUPER,
  TK_KEYWORD_SWITCH,
  TK_KEYWORD_THIS,
  TK_KEYWORD_THROW,
  TK_KEYWORD_TRUE,
  TK_KEYWORD_TRY,
  TK_KEYWORD_TYPEOF,
  TK_KEYWORD_UNDEFINED,
  TK_KEYWORD_USING,
  TK_KEYWORD_VAR,
  TK_KEYWORD_VOID,
  TK_KEYWORD_WHILE,
  TK_KEYWORD_WITH,
  TK_KEYWORD_YIELD,
  TK_KEYWORD_END = TK_KEYWORD_YIELD,
  TK_IDENTIFIER,
  TK_PRIVATE_IDENTIFIER,
} bali_token_kind_t;

/**
 * @brief Token with its span, kind, and whitespace/newline flags.
 */
typedef struct bali_token_t {
  bali_span_t		span;
  bali_token_kind_t	kind;
  bool			leading_ws;
  bool			leading_nl;
} bali_token_t;

/**
 * @brief Tokenizer state. References the caller-owned source buffer. The owner of the buffer
 * is responsible to free it.
 */
typedef struct bali_lexer_t {
  const char		*src;
  bsize_t		 src_len;
  bali_position_t	 last_token_position;
  bali_token_t		 current_token;
} bali_lexer_t;

/**
 * @brief Initialize a lexer from a buffer.
 */
BALI_API void bali_lexer_setup(bali_lexer_t *lexer, const char *src, bsize_t src_len);
/**
 * @brief Initialize a lexer from a null-terminated string.
 */
BALI_API void bali_lexer_setup_from_cstring(bali_lexer_t *lexer, BALI_LCSTR src);
/**
 * @brief Advance to the next token. Writes it to @p token.
 */
BALI_API bool bali_lexer_next_token(bali_lexer_t *lexer, bali_token_t **token);
/**
 * @brief Copies the slice of the source code to @p output if it has enough @p capacity.
 */
BALI_API bool bali_token_cstr(bali_lexer_t *lexer, bali_token_t *token, char *output, bsize_t capacity);
