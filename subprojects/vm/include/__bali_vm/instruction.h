#pragma once

#include <bali_utilities.h>

typedef enum bali_instruction_bytecode_t : uint16_t {
  I_GET_GLOBAL_THIS,
  I_GET_OBJECT_FIELD,
  I_INVOKE_DYNAMIC,
  I_PUSH,
  I_POP,
  I_LOADSTR,
  I_LOADBLN,
  I_JUMP,
  I_CJUMP,
} bali_instruction_bytecode_t;

typedef enum bali_vm_register_t : uint8_t {
  R_I64_1 = 0,
  R_I64_2,
  R_I64_3,
  R_I64_4,
  R_F64_1 = 50,
  R_F64_2,
  R_F64_3,
  R_F64_4,
  R_PTR_1 = 100,
  R_PTR_2,
  R_PTR_3,
  R_PTR_4,
  R_THIS = 253,
  R_NONE = 254
} bali_vm_register_t;

typedef struct bali_instruction_t {
  bali_instruction_bytecode_t bc;
  bali_vm_register_t op1, op2, op3;
  union {
    char *string;
    bsize_t index;
    bool boolean;
  } constant;
  bali_vm_register_t out;
} bali_instruction_t;


typedef struct bali_bytecode_builder_t {
  bali_instruction_t	*instructions;
  bsize_t		 length;
  bsize_t		 capacity;
} bali_bytecode_builder_t;

BALI_API bool bali_bytecode_builder_setup(bali_bytecode_builder_t *bc);
BALI_API void bali_bytecode_builder_free(bali_bytecode_builder_t *bc);
BALI_API void bali_bytecode_get_global_this(bali_bytecode_builder_t *bc, bali_vm_register_t out);
BALI_API void bali_bytecode_get_object_field(bali_bytecode_builder_t *bc, bali_vm_register_t obj, bali_vm_register_t field, bali_vm_register_t out);
BALI_API void bali_bytecode_invoke_dynamic(bali_bytecode_builder_t *bc, bali_vm_register_t fn, bali_vm_register_t out);
BALI_API void bali_bytecode_push(bali_bytecode_builder_t *bc, bali_vm_register_t in);
BALI_API void bali_bytecode_pop(bali_bytecode_builder_t *bc, bali_vm_register_t out);
BALI_API void bali_bytecode_loadstr(bali_bytecode_builder_t *bc, char *str, bali_vm_register_t out);
BALI_API void bali_bytecode_loadbln(bali_bytecode_builder_t *bc, bool value, bali_vm_register_t out);
BALI_API bali_instruction_t *bali_bytecode_jump(bali_bytecode_builder_t *bc);
BALI_API bali_instruction_t *bali_bytecode_cjump(bali_bytecode_builder_t *bc, bali_vm_register_t in);
BALI_API bsize_t bali_bytecode_current_position(bali_bytecode_builder_t *bc);
BALI_API void bali_bytecode_dump(bali_bytecode_builder_t *bc, FILE *output);
