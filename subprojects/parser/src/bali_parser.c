#include <bali_parser.h>
#include <bali_utilities.h>
#include <bali_tokenizer.h>
#include <stdint.h>
#include <string.h>

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
    PS_IN_IF_START,
    PS_IN_IF_CONDITION,
    PS_IN_IF_BODY,
    PS_IN_ELSE_BODY,
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

  bali_instruction_t *if_end_jump = nullptr;
  bali_instruction_t *else_jump = nullptr;
  bsize_t else_block_index = 0;
  //bali_instruction_t *else_end_jump = nullptr;
  int_fast8_t after_if_body = 0;
  
#define parse_error(err) do { return; } while(0);

  while (bali_lexer_next_token(lexer, &current) && current != nullptr) {
    BALI_DCHECK(state[state_it] != PS_NONE);

    if (after_if_body > 0) {
      after_if_body--;
    }

    if (current->kind == TK_IDENTIFIER) {
      state[state_it] = PS_IN_EXPRESSION;
      bali_token_cstr(lexer, current, str, 256);
      bali_bytecode_get_global_this(p->bc, R_THIS);
      bali_bytecode_loadstr(p->bc, str, R_PTR_1);
      memset(str, 0, 256);
      bali_bytecode_get_object_field(p->bc, R_THIS, R_PTR_1, R_PTR_2);
    } else if (current->kind == TK_PUNCT_OPEN_PARENTHESIS) {
      if (state[state_it] == PS_IN_EXPRESSION) {
	state[state_it] = PS_IN_FUNCALL;
	state_it += 1;
      } else if (state[state_it] == PS_IN_IF_START) {
	state[state_it] = PS_IN_IF_CONDITION;
	state_it += 1;
      }
    } else if (current->kind == TK_PUNCT_CLOSING_PARENTHESIS) {
      if (state_it == 0) {
	parse_error("Unmatching closing parenthesis.");
      }

      state_it -= 1;
      if (state[state_it] == PS_IN_FUNCALL) {
       	bali_bytecode_invoke_dynamic(p->bc, R_PTR_2, R_PTR_1);
	if (state_it > 0 && state[state_it - 1] == PS_IN_IF_BODY) {
	  if_end_jump = bali_bytecode_jump(p->bc);
	  --state_it;
	  if (else_jump != nullptr) {
	    else_jump->constant.index = bali_bytecode_current_position(p->bc);
	    else_jump = nullptr;
	  } else if (if_end_jump != nullptr) {
	    if_end_jump->constant.index = else_block_index + 1; // skip the actual else
	    if_end_jump = nullptr;
	    else_block_index = 0;
	  }
	}
      } else if (state[state_it] == PS_IN_IF_CONDITION) {
	bali_instruction_t *inst = bali_bytecode_cjump(p->bc, R_PTR_1);
	state[state_it] = PS_IN_IF_BODY;
	state[++state_it] = PS_START;
	else_jump = bali_bytecode_jump(p->bc);
	inst->constant.index = bali_bytecode_current_position(p->bc); // skip the else jump
	else_block_index = inst->constant.index;
      } else if (state[state_it] == PS_IN_IF_BODY) {
	BALI_DCHECK(else_jump != nullptr);
	BALI_DCHECK(state_it > 0);
	else_jump->constant.index = bali_bytecode_current_position(p->bc) + 1;
	state_it--;
	after_if_body = 2;
      }
    } else if (current->kind == TK_SINGLE_QUOTED_STRING || current->kind == TK_DOUBLE_QUOTED_STRING) {
      if (state_it > 0 && state[state_it - 1] == PS_IN_FUNCALL) {
	state[state_it] = PS_IN_EXPRESSION;
	bali_token_cstr(lexer, current, str, 256);
	bali_bytecode_loadstr(p->bc, str, R_PTR_1);
	memset(str, 0, 256);
	bali_bytecode_push(p->bc, R_PTR_1);
      }
    } else if (current->kind == TK_PUNCT_COMMA) {
      if (state_it > 0 && state[state_it - 1] == PS_IN_FUNCALL && state[state_it] == PS_IN_EXPRESSION) {
	continue;
      }
    } else if (current->kind == TK_KEYWORD_IF) {
      state[state_it] = PS_IN_IF_START;
    } else if (current->kind == TK_KEYWORD_TRUE || current->kind == TK_KEYWORD_FALSE) {
      bali_bytecode_loadbln(p->bc, current->kind == TK_KEYWORD_TRUE, R_PTR_1);
    } else if ((after_if_body > 0 || state[state_it] == PS_IN_IF_BODY) && current->kind == TK_KEYWORD_ELSE) {
      state[state_it] = PS_IN_ELSE_BODY;
      state[++state_it] = PS_START;
    }
  }

#undef parse_error

}
