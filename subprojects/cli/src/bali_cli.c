#include "__bali_vm/shard.h"
#include <bali_cli.h>
#include <bali_tokenizer.h>
#include <bali_parser.h>
#include <bali_vm.h>
#include <bali_utilities.h>
#include <bali_utilities/bump_arena.h>

int main()
{
  bali_lexer_t lexer;
  bali_parser_t parser;
  bali_bytecode_builder_t bc;
  bali_vm_context_t context;
  bali_vm_shard_t shard;
  bali_bump_arena_t bump;
  
  bali_lexer_setup_from_cstring(&lexer, "hello");
  bali_bytecode_builder_setup(&bc);
  bali_bump_arena_init(&bump, 4096);
  bali_vm_shard_init(&shard, &context, &bc, &bump);
  
  bali_parser_setup(&parser, &bc);
  bali_parser_parse(&parser, &lexer);

  bali_vm_context_setup(&context);
  bali_vm_shard_setup(&shard, nullptr, SHARD_NORMAL, 0);
  bali_vm_shard_execute(&shard);
  
  //bali_vm_value_t *value = bali_vm_context_execute(&context, &bc);
  
  //char output[256] = {0};
  //bali_vm_value_cstr(value, output, 256);
  
  //printf("Result of the execution: %s\n", output);
  return 0;
}
