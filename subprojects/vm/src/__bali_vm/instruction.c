#include <bali_utilities.h>
#include <bali_vm.h>
#include <stdlib.h>

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
  
bali_bytecode_builder_t *bali_bytecode_builder_new()
{
  bali_bytecode_builder_t *bc = malloc(sizeof(*bc));
  BALI_RCHECK_NOT_NULL(bc);

  bc->capacity = 4096;
  bc->length = 0;
  bc->instructions = malloc(sizeof(bali_instruction_t) * bc->capacity);
  BALI_RCHECK_NOT_NULL(bc->instructions);

  return bc;
}

void bali_bytecode_builder_free(bali_bytecode_builder_t *bc)
{
  BALI_DCHECK(bc != nullptr);
  BALI_DCHECK(bc->instructions != nullptr);
  
  free(bc->instructions);
  free(bc);
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
