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

typedef enum parser_state_t : uint_fast8_t {
  PS_NONE = 0,
    PS_START,
    PS_IN_EXPRESSION,
    PS_IN_FUNCALL,
    } parser_state_t;

void bali_parser_parse(bali_parser_t *p, bali_lexer_t *lexer)
{
  BALI_DCHECK(p != nullptr);
  BALI_DCHECK(lexer != nullptr);

  parser_state_t state[32] = { PS_NONE };
  uint_fast8_t state_it = 0;
  
  //bali_token_t tail[8] = {0};
  //int tail_size = 0;

  char str[256];
  
  bali_token_t *current;
  state[state_it] = PS_START;

#define parse_error(err) do { return; } while(0);

  while (bali_lexer_next_token(lexer, &current) && current != nullptr) {
    if (current->kind == TK_IDENTIFIER) {
      if (state[state_it] == PS_START) {
	state[state_it] = PS_IN_EXPRESSION;
	bali_token_cstr(lexer, current, str, 256);
	bali_bytecode_get_global_this(p->bc, R_THIS);
	bali_bytecode_loadstr(p->bc, str, R_PTR_1);
	bali_bytecode_get_object_field(p->bc, R_THIS, R_PTR_1, R_PTR_2);
      }
    } else if (current->kind == TK_PUNCT_OPEN_PARENTHESIS) {
      if (state[state_it] == PS_IN_EXPRESSION) {
	state[state_it] = PS_IN_FUNCALL;
	state_it += 1;
      }
    } else if (current->kind == TK_PUNCT_CLOSING_PARENTHESIS) {
      if (state_it == 0) {
	parse_error("Unmatching closing parenthesis.");
	return;
      }

      state_it -= 1;
      if (state[state_it] == PS_IN_FUNCALL) {
       	bali_bytecode_invoke_dynamic(p->bc, R_PTR_2, R_PTR_1);
      }
    } else if (current->kind == TK_SINGLE_QUOTED_STRING || current->kind == TK_DOUBLE_QUOTED_STRING) {
      if (state_it > 0 && state[state_it - 1] == PS_IN_FUNCALL) {
	state[state_it] = PS_IN_EXPRESSION;
	bali_token_cstr(lexer, current, str, 256);
	bali_bytecode_loadstr(p->bc, str, R_PTR_1);
	bali_bytecode_push(p->bc, R_PTR_1);
      }
    } else if (current->kind == TK_PUNCT_COMMA) {
      if (state_it > 0 && state[state_it - 1] == PS_IN_FUNCALL && state[state_it] == PS_IN_EXPRESSION) {
	continue;
      }
    }
  }

#undef parse_error

}
