#include "__bali_vm/value.h"
#include <bali_utilities.h>
#include <bali_utilities/bump_arena.h>
#include <bali_vm.h>
#include <stdio.h>
#include <string.h>

void bali_vm_shard_init(bali_vm_shard_t *shard,
			bali_vm_context_t *context,
			bali_bytecode_builder_t *bc,
			bali_bump_arena_t *arena)
{
  BALI_DCHECK(shard != nullptr);
  BALI_DCHECK(context != nullptr);
  BALI_DCHECK(bc != nullptr);
  BALI_DCHECK(arena != nullptr);

  memset(shard, 0, sizeof(*shard));
  shard->context = context;
  shard->bc = bc;
  shard->bump = arena;
  
  bali_bump_arena_recycle(shard->bump);
}

void bali_vm_shard_setup(bali_vm_shard_t *shard, bali_vm_shard_t *parent, bali_vm_shard_type_t type, bsize_t pc)
{
  BALI_DCHECK(shard != nullptr);
  BALI_DCHECK_BSIZE_BOUNDS(pc);
  BALI_DCHECK(shard->state != SHARD_STATE_RUNNING);
  BALI_RCHECK(pc < shard->bc->length);

  bali_bump_arena_recycle(shard->bump);
  
  shard->parent = parent;
  shard->type = type;
  shard->pc = pc;
  shard->export_count = 0;
  shard->exports = bali_bump_arena_malloc(shard->bump, sizeof(*shard->exports) * 64);
  BALI_RCHECK_NOT_NULL(shard->exports);
  shard->export_capacity = 64;
  shard->stack_capacity = 4096;
  shard->stack = bali_bump_arena_malloc(shard->bump, shard->stack_capacity);
  BALI_RCHECK_NOT_NULL(shard->stack);
  shard->state = SHARD_STATE_IDLE;
}

void bali_vm_shard_absorb_in_parent(bali_vm_shard_t *shard)
{
  BALI_DCHECK(shard != nullptr);
  BALI_DCHECK(shard->parent != nullptr);
  BALI_DCHECK(shard->state == SHARD_STATE_COMPLETED);

  // implement logic to copy exports to parent
  shard->state = SHARD_STATE_COMMITTED;
}

void bali_vm_shard_execute(bali_vm_shard_t *shard)
{
  BALI_DCHECK(shard != nullptr);
  BALI_DCHECK(shard->state == SHARD_STATE_IDLE || shard->state == SHARD_STATE_YIELDED);

  bali_vm_value_t *vt[12] = {0};

  for (int i = 0; i < 12; i++) {
    vt[i] = shard->context->undefined;
  }
  
  bali_vm_value_t *this = shard->context->undefined;
  void *stack_ptr = shard->stack;
  
  while (shard->pc < shard->bc->length) {
    bali_instruction_t *i = &shard->bc->instructions[shard->pc];
    switch (i->bc) {
    case I_GET_GLOBAL_THIS:
      BALI_DCHECK(i->out >= R_PTR_1 && i->out <= R_PTR_4);
      vt[i->out] = shard->context->global_this;
      shard->pc++;
      break;
    case I_GET_OBJECT_FIELD:
      BALI_DCHECK(i->op1 >= R_PTR_1 && i->op1 <= R_PTR_4);
      BALI_DCHECK(i->op2 >= R_PTR_1 && i->op2 <= R_PTR_4);
      BALI_DCHECK(i->op2 >= R_PTR_1 && i->op2 <= R_PTR_4);
      BALI_DCHECK(i->op1 != i->op2);
      BALI_DCHECK(i->out >= R_PTR_1 && i->out <= R_PTR_4);

      bali_vm_value_t *obj = vt[i->op1];
      bali_vm_value_t *field_id = vt[i->op2];

      BALI_DCHECK(obj->kind == BALI_VM_VALUE_OBJECT);
      BALI_DCHECK(field_id->kind == BALI_VM_VALUE_STRING);

      for (int p = 0; p < obj->obj.prop_len; p++) {
	bali_vm_key_value_pair_t *prop = &obj->obj.own_properties[p];
	if (prop->key->kind == BALI_VM_VALUE_STRING) {
	  char a[256];
	  char b[256];

	  bali_vm_string_cstr(&prop->key->string, a, 256);
	  bali_vm_string_cstr(&field_id->string, b, 256);
	  
	  if (bali_vm_string_cmp(&prop->key->string, &field_id->string) == 0) {
	    vt[i->out] = prop->value;
	    break;
	  }
	}
      }
      
      shard->pc++;
      break;
    case I_INVOKE_DYNAMIC:
      BALI_DCHECK(i->op1 >= R_PTR_1 && i->op1 <= R_PTR_4);
      bali_vm_value_t *fn = vt[i->op1];
      BALI_DCHECK(fn != nullptr);
      BALI_DCHECK(fn->kind == BALI_VM_VALUE_FUNCTION);
      if (fn->fn.is_native) {
	bali_vm_scope_t scope = {0};
	memcpy(scope.vt, vt, sizeof(bali_vm_value_t *) * 12);
	scope.this = this;
	scope.stack = shard->stack;
	scope.stack_ptr = stack_ptr;

	fn->fn.fn.native(&scope);
	memcpy(vt, scope.vt, sizeof(bali_vm_value_t *) * 12);
	this = scope.this;
	stack_ptr = scope.stack_ptr;
      } else {
	puts("NOT A REAL FUNCTION");
      }
      shard->pc++;
      break;
    case I_PUSH:
      BALI_DCHECK(i->out >= R_PTR_1 && i->op1 <= R_PTR_4);
      BALI_RCHECK((bsize_t) stack_ptr < ((bsize_t) shard->stack + shard->stack_capacity));
      
      bali_vm_value_t *reg = vt[i->op1];
      memcpy(stack_ptr, reg, sizeof(*reg));
      stack_ptr = ((char *)stack_ptr) + sizeof(*reg);
      shard->pc++;
      break;
    case I_POP:
      BALI_DCHECK(i->out >= R_PTR_1 && i->op1 <= R_PTR_4);
      BALI_RCHECK((bsize_t) stack_ptr < ((bsize_t) shard->stack + shard->stack_capacity));
      
      stack_ptr = ((char *)stack_ptr) - sizeof(*reg);
      stack_ptr = stack_ptr < shard->stack ? shard->stack : stack_ptr;
      shard->pc++;
      break;
    case I_LOADSTR:
      BALI_DCHECK(i->out >= R_PTR_1 && i->op1 <= R_PTR_4);
      BALI_RCHECK((bsize_t) stack_ptr < ((bsize_t) shard->stack + shard->stack_capacity));

      bali_vm_value_t *value = vt[i->out] = bali_bump_arena_malloc(shard->bump, sizeof(*value));
      BALI_RCHECK_NOT_NULL(value);
      
      value->kind = BALI_VM_VALUE_STRING;
      bali_vm_string_set(&value->string, i->constant, strlen(i->constant));
      vt[i->out] = value;
      shard->pc++;
      break;
    }
  }
  
  shard->state = SHARD_STATE_COMPLETED;
}

bali_vm_shard_state_t bali_vm_shard_state(bali_vm_shard_t *shard)
{
  BALI_DCHECK(shard != nullptr);
  return shard->state;
}
