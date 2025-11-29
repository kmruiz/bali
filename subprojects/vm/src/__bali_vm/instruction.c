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
  BALI_DCHECK(str != nullptr);
  
  bsize_t it = __ensure_enough_memory(bc);

  bc->instructions[it].bc = I_LOADSTR;
  bc->instructions[it].constant.string = strdup(str); // TODO: this leaks memory, we should not do this
  bc->instructions[it].out = out;
}

BALI_API void bali_bytecode_loadbln(bali_bytecode_builder_t *bc, bool value, bali_vm_register_t out)
{
  BALI_DCHECK(bc != nullptr);
  bsize_t it = __ensure_enough_memory(bc);

  bc->instructions[it].bc = I_LOADBLN;
  bc->instructions[it].constant.boolean = value;
  bc->instructions[it].out = out;
}

bali_instruction_t *bali_bytecode_jump(bali_bytecode_builder_t *bc)
{
  BALI_DCHECK(bc != nullptr);

  bsize_t it = __ensure_enough_memory(bc);
  bc->instructions[it].bc = I_JUMP;
  bc->instructions[it].constant.index = -1;
  return &bc->instructions[it];
}

bali_instruction_t *bali_bytecode_cjump(bali_bytecode_builder_t *bc, bali_vm_register_t in)
{
  BALI_DCHECK(bc != nullptr);

  bsize_t it = __ensure_enough_memory(bc);
  bc->instructions[it].bc = I_CJUMP;
  bc->instructions[it].op1 = in;
  bc->instructions[it].constant.index = -1;

  return &bc->instructions[it];
}

bsize_t bali_bytecode_current_position(bali_bytecode_builder_t *bc)
{
  BALI_DCHECK(bc != nullptr);
  return bc->length;
}

static void print_register_name(bali_vm_register_t reg, FILE *output)
{
  BALI_DCHECK(output != nullptr);
  char *reg_name;
  switch (reg) {
  case R_I64_1:
    reg_name = "i64_1";
    break;
  case R_I64_2:
    reg_name = "i64_2";
    break;
  case R_I64_3:
    reg_name = "i64_3";
    break;
  case R_I64_4:
    reg_name = "i64_4";
    break;
  case R_F64_1:
    reg_name = "f64_1";
    break;
  case R_F64_2:
    reg_name = "f64_2";
    break;
  case R_F64_3:
    reg_name = "f64_3";
    break;
  case R_F64_4:
    reg_name = "f64_4";
    break;
  case R_PTR_1:
    reg_name = "ptr_1";
    break;
  case R_PTR_2:
    reg_name = "ptr_2";
    break;
  case R_PTR_3:
    reg_name = "ptr_3";
    break;
  case R_PTR_4:
    reg_name = "ptr_4";
    break;
  case R_THIS:
    reg_name = "this";
    break;
  case R_NONE:
    reg_name = "NONE";
    break;
  }
  fprintf(output, "%-10s", reg_name);
}

void bali_bytecode_dump(bali_bytecode_builder_t *bc, FILE *output)
{
  BALI_DCHECK(bc != nullptr);
  BALI_DCHECK(output != nullptr);

  for (bsize_t it = 0; it < bc->length; it++) {
    bali_instruction_t *i = &bc->instructions[it];

    fprintf(output, "%05ld ", it);
    switch (i->bc) {
    case I_GET_GLOBAL_THIS:
      fprintf(output, "%-20s", "get_global_this");
      print_register_name(i->out, output);
      break;
    case I_GET_OBJECT_FIELD:
      fprintf(output, "%-20s", "get_obj_field");
      print_register_name(i->op1, output);
      print_register_name(i->op2, output);
      print_register_name(i->out, output);
      break;
    case I_INVOKE_DYNAMIC:
      fprintf(output, "%-20s", "invoke_dynamic");
      print_register_name(i->op1, output);
      print_register_name(i->out, output);
      break;
    case I_PUSH:
      fprintf(output, "%-20s", "push");
      print_register_name(i->op1, output);
      break;
    case I_POP:
      fprintf(output, "%-20s", "pop");
      print_register_name(i->out, output);
      break;
    case I_LOADSTR:
      fprintf(output, "%-20s", "loadstr");
      print_register_name(i->out, output);
      fprintf(output, "<%s>", bc->instructions[it].constant.string);
      break;
    case I_LOADBLN:
      fprintf(output, "%-20s", "loadbln");
      print_register_name(i->out, output);
      fprintf(output, "<%s>", bc->instructions[it].constant.boolean ? "true" : "false");
      break;
    case I_JUMP:
      fprintf(output, "%-20s", "jump");
      fprintf(output, "%05lu", i->constant.index);
      break;
    case I_CJUMP:
      fprintf(output, "%-20s", "cjump");
      print_register_name(i->op1, output);
      fprintf(output, "%05lu", i->constant.index);
      break;
    }
    fprintf(output, "\n");
  }
}
