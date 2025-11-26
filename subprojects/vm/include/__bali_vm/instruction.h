#pragma once

#include <bali_utilities.h>

typedef enum bali_instruction_bytecode_t : uint16_t {
  I_GET_GLOBAL_THIS,
  I_GET_OBJECT_FIELD,
  I_INVOKE_DYNAMIC,
} bali_instruction_bytecode_t;

typedef enum bali_vm_register_t : uint8_t {
  I64_1 = 0,
  I64_2,
  I64_3,
  I64_4,
  F64_1 = 50,
  F64_2,
  F64_3,
  F64_4,
  PTR_1 = 100,
  PTR_2,
  PTR_3,
  PTR_4,
  THIS = 253,
  NONE = 254
} bali_vm_register_t;

typedef struct bali_instruction_t {
  bali_instruction_bytecode_t bc;
  bali_vm_register_t op1, op2, op3;
  bali_vm_register_t out;
} bali_instruction_t;


typedef struct bali_bytecode_builder_t {
  bali_instruction_t	*instructions;
  bsize_t		 length;
  bsize_t		 capacity;
} bali_bytecode_builder_t;

BALI_API bali_bytecode_builder_t *bali_bytecode_builder_new();
BALI_API void bali_bytecode_builder_free(bali_bytecode_builder_t *bc);
BALI_API void bali_bytecode_get_global_this(bali_bytecode_builder_t *bc, bali_vm_register_t out);
BALI_API void bali_bytecode_get_object_field(bali_bytecode_builder_t *bc, bali_vm_register_t obj, bali_vm_register_t field, bali_vm_register_t out);
BALI_API void bali_bytecode_invoke_dynamic(bali_bytecode_builder_t *bc, bali_vm_register_t fn, bali_vm_register_t out);
