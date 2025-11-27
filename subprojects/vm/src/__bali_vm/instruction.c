#include <bali_utilities.h>
#include <bali_vm.h>
#include <stdlib.h>
#include <string.h>

static inline bsize_t __ensure_enough_memory(bali_bytecode_builder_t *bc)
{
  bsize_t it = bc->length++;
  if (it >= bc->capacity) {
    bsize_t new_capacity = bc->capacity * 2;
    bc->instructions = realloc(bc->instructions, new_capacity);
    BALI_RCHECK_NOT_NULL(bc->instructions);
  }

  return it;
}

bool bali_bytecode_builder_setup(bali_bytecode_builder_t *bc)
{
  BALI_DCHECK(bc != nullptr);

  bc->capacity = 4096;
  bc->length = 0;
  bc->instructions = malloc(sizeof(bali_instruction_t) * bc->capacity);
  if (bc->instructions == nullptr) {
    bc->capacity = 0;
    return false;
  }

  return true;
}

void bali_bytecode_builder_free(bali_bytecode_builder_t *bc)
{
  BALI_DCHECK(bc != nullptr);
  BALI_DCHECK(bc->instructions != nullptr);
  
  free(bc->instructions);
}

void bali_bytecode_get_global_this(bali_bytecode_builder_t *bc, bali_vm_register_t out)
{
  BALI_DCHECK(bc != nullptr);
  bsize_t it = __ensure_enough_memory(bc);
  bc->instructions[it].bc = I_GET_GLOBAL_THIS;
  bc->instructions[it].out = out; 
}

void bali_bytecode_get_object_field(bali_bytecode_builder_t *bc, bali_vm_register_t obj, bali_vm_register_t field, bali_vm_register_t out)
{
  BALI_DCHECK(bc != nullptr);
  bsize_t it = __ensure_enough_memory(bc);
  bc->instructions[it].bc = I_GET_OBJECT_FIELD;
  bc->instructions[it].op1 = obj;
  bc->instructions[it].op2 = field;
  bc->instructions[it].out = out;
}

void bali_bytecode_invoke_dynamic(bali_bytecode_builder_t *bc, bali_vm_register_t fn, bali_vm_register_t out)
{
  BALI_DCHECK(bc != nullptr);
  bsize_t it = __ensure_enough_memory(bc);
  bc->instructions[it].bc = I_INVOKE_DYNAMIC;
  bc->instructions[it].op1 = fn;
  bc->instructions[it].out = out;
}

void bali_bytecode_push(bali_bytecode_builder_t *bc, bali_vm_register_t in)
{
  BALI_DCHECK(bc != nullptr);
  bsize_t it = __ensure_enough_memory(bc);
  bc->instructions[it].bc = I_PUSH;
  bc->instructions[it].op1 = in;
}

void bali_bytecode_pop(bali_bytecode_builder_t *bc, bali_vm_register_t out)
{
  BALI_DCHECK(bc != nullptr);
  bsize_t it = __ensure_enough_memory(bc);
  bc->instructions[it].bc = I_POP;
  bc->instructions[it].out = out;
}

void bali_bytecode_loadstr(bali_bytecode_builder_t *bc, char *str, bali_vm_register_t out)
{
  BALI_DCHECK(bc != nullptr);
  bsize_t it = __ensure_enough_memory(bc);

  bc->instructions[it].bc = I_LOADSTR;
  bc->instructions[it].constant = strdup(str); // TODO: this leaks memory, we should not do this
  bc->instructions[it].out = out;
}  
