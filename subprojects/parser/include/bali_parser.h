#pragma once

#include <bali_utilities.h>
#include <bali_tokenizer.h>
#include <bali_vm.h>

typedef struct bali_lexical_scope_t {
  struct bali_lexical_scope_t *parent;
} bali_lexical_scope_t;

typedef struct bali_parser_t {
  bali_lexical_scope_t *root_lexical_scope;
  bali_bytecode_builder_t *bc;
} bali_parser_t;

BALI_API void bali_parser_setup(bali_parser_t *p, bali_bytecode_builder_t *bc);
BALI_API void bali_parser_free(bali_parser_t *p);
BALI_API void bali_parser_parse(bali_parser_t *p, bali_lexer_t *lexer);
