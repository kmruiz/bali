#include "__bali_vm/instruction.h"
#include <bali_parser.h>
#include <bali_utilities.h>
#include <bali_tokenizer.h>

void bali_parser_setup(bali_parser_t *p, bali_bytecode_builder_t *bc)
{
  BALI_DCHECK(p != nullptr);
  BALI_DCHECK(bc != nullptr);

  p->root_lexical_scope = nullptr;
  p->bc = bc;
}

void bali_parser_free(bali_parser_t *p)
{
  BALI_DCHECK(p != nullptr);
  while (p->root_lexical_scope != nullptr) {
    bali_lexical_scope_t *parent = p->root_lexical_scope->parent;
    free(p->root_lexical_scope);
    p->root_lexical_scope = parent;
  }
}

typedef enum parser_state_t {
  PS_START,
  PS_IN_EXPRESSION,
} parser_state_t;

void bali_parser_parse(bali_parser_t *p, bali_lexer_t *lexer)
{
  BALI_DCHECK(p != nullptr);
  BALI_DCHECK(lexer != nullptr);

  parser_state_t state = PS_START;
  //bali_token_t tail[8] = {0};
  //int tail_size = 0;

  char str[256];
  
  bali_token_t *current;
  while (bali_lexer_next_token(lexer, &current)) {
    if (current->kind == TK_IDENTIFIER) {
      if (state == PS_START) {
	state = PS_IN_EXPRESSION;
	bali_token_cstr(lexer, current, str, 256);
	bali_bytecode_get_global_this(p->bc, R_THIS);
	bali_bytecode_loadstr(p->bc, str, R_PTR_1);
	bali_bytecode_get_object_field(p->bc, R_THIS, R_PTR_1, R_PTR_2);
	bali_bytecode_invoke_dynamic(p->bc, R_PTR_2, R_PTR_1);
      }
    }
  }
}
