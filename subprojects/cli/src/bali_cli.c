#include "__bali_vm/shard.h"
#include "__bali_vm/value.h"
#include <bali_cli.h>
#include <bali_tokenizer.h>
#include <bali_parser.h>
#include <bali_vm.h>
#include <bali_utilities.h>
#include <bali_utilities/bump_arena.h>

void print_hello_world(bali_vm_scope_t *scope)
{
  BALI_DCHECK(scope != nullptr);
  
  puts("Hello World!");
}

int main()
{
  bali_lexer_t lexer;
  bali_parser_t parser;
  bali_bytecode_builder_t bc;
  bali_vm_context_t context;
  bali_vm_shard_t shard;
  bali_bump_arena_t bump;
  bali_vm_value_t global_this;
  bali_vm_key_value_pair_t prop;
  bali_vm_value_t hello_world_key_str;
  hello_world_key_str.kind = BALI_VM_VALUE_STRING;
  bali_vm_string_set(&hello_world_key_str.string, "hello", 5);
  bali_vm_value_t hello_world_fn;
  hello_world_fn.kind = BALI_VM_VALUE_FUNCTION;
  hello_world_fn.fn.name = &hello_world_key_str;
  hello_world_fn.fn.is_native = true;
  hello_world_fn.fn.fn.native = print_hello_world;
  
  global_this.kind = BALI_VM_VALUE_OBJECT;
  prop.key = &hello_world_key_str;
  prop.value = &hello_world_fn;
  global_this.obj.own_properties = &prop;
  global_this.obj.prop_len = 1;
  global_this.obj.prop_capacity = 1;
  
  bali_lexer_setup_from_cstring(&lexer, "hello");
  bali_bytecode_builder_setup(&bc);
  bali_bump_arena_init(&bump, 8192);
  bali_vm_shard_init(&shard, &context, &bc, &bump);
  
  bali_parser_setup(&parser, &bc);
  bali_parser_parse(&parser, &lexer);

  bali_vm_context_setup(&context);
  context.global_this = &global_this;
  
  bali_vm_shard_setup(&shard, nullptr, SHARD_NORMAL, 0);
  bali_vm_shard_execute(&shard);
  
  //bali_vm_value_t *value = bali_vm_context_execute(&context, &bc);
  
  //char output[256] = {0};
  //bali_vm_value_cstr(value, output, 256);
  
  //printf("Result of the execution: %s\n", output);
  return 0;
}
