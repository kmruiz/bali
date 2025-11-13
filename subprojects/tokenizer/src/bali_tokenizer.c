#include <ctype.h>
#include <string.h>

#include <bali_utilities.h>
#include <bali_tokenizer.h>

static inline void update_position_based_on_char(bali_position_t *t, char ch) {
  BALI_DCHECK(t != NULL);
  
  t->index++;
  if (ch == '\n') {
    t->line++;
    t->column = 1;
  } else {
    t->column++;
  }
}
void bali_lexer_setup(bali_lexer_t *lexer, const char *src, bsize_t src_len)
{
  BALI_DCHECK(lexer != NULL);
  BALI_DCHECK(src != NULL);
  BALI_DCHECK(src_len > 0);
  
  memset(lexer, 0, sizeof(*lexer));
  lexer->last_token_position.line = lexer->last_token_position.column = 1;
  lexer->src = src;
  lexer->src_len = src_len;
}

void bali_lexer_setup_from_cstring(bali_lexer_t *lexer, BALI_LCSTR src)
{
  BALI_DCHECK(lexer != NULL);
  BALI_DCHECK(src != NULL);

  bali_lexer_setup(lexer, src, strlen(src));
}

bool bali_lexer_next_token(bali_lexer_t *lexer, bali_token_t **token)
{
  BALI_DCHECK(lexer != NULL);
  BALI_DCHECK(token != NULL);

  if (lexer->last_token_position.index >= lexer->src_len) {
    *token = NULL;
    return false;
  }

  lexer->current_token.leading_nl = lexer->current_token.leading_ws = false;
  lexer->current_token.kind = TK_EOF;

  bali_position_t start = lexer->last_token_position;
  bali_position_t *it = &lexer->last_token_position;
  unsigned char ch = ch = lexer->src[it->index];
  
  while (isspace(ch) && it->index < lexer->src_len) {
    ch = lexer->src[it->index];
    lexer->current_token.leading_ws = lexer->current_token.leading_ws || ch == ' ' || ch == '\t';
    lexer->current_token.leading_nl = lexer->current_token.leading_nl || ch == '\n';
    update_position_based_on_char(it, ch);
  }

  start = *it;
  if (it->index >= lexer->src_len) {
    lexer->current_token.kind = TK_EOF;
    lexer->current_token.span.start = start;
    lexer->current_token.span.end = *it;
    *token = &lexer->current_token;
    return true;
  }
  
  if (isalpha(ch) || ch == '_' || ch == '#') { 
    lexer->current_token.kind = ch == '#' ? TK_PRIVATE_IDENTIFIER : TK_IDENTIFIER;
    update_position_based_on_char(it, ch);
    ch = lexer->src[it->index];
    
    while ((isalnum(ch) || ch == '_') && it->index < lexer->src_len) {
      ch = lexer->src[it->index];
      update_position_based_on_char(it, ch);
    }
    lexer->current_token.span.start = start;
    lexer->current_token.span.end = *it;
    *token = &lexer->current_token;
    return true;
  }
  
  return false;
}

bool bali_token_cstr(bali_lexer_t *lexer, bali_token_t *token, char *output, bsize_t capacity)
{
  BALI_DCHECK(lexer != NULL);
  BALI_DCHECK(token != NULL);
  BALI_DCHECK(output != NULL);
  BALI_DCHECK(capacity > 0);
  BALI_DCHECK(token->span.end.index > 0);
  BALI_DCHECK(token->span.end.index <= lexer->src_len);
  BALI_DCHECK(token->span.start.index >= 0);
  BALI_DCHECK(token->span.end.index > token->span.start.index);
  
  const bsize_t required_capacity = (token->span.end.index - token->span.start.index) + 1; // for the ending '\0'
  BALI_DCHECK_BSIZE_BOUNDS(required_capacity);

  if (capacity < required_capacity) {
    return false;
  }

  memcpy(output, lexer->src + token->span.start.index, required_capacity);
  output[required_capacity] = 0;
  return true;
}
