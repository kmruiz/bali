#pragma once

#include <bali_vm.h>
#include <bali_utilities/bump_arena.h>
#include <bali_utilities.h>

typedef enum bali_vm_shard_type_t {
  SHARD_NORMAL = 0, SHARD_SPECULATIVE
} bali_vm_shard_type_t;

typedef enum bali_vm_shard_state_t {
  SHARD_STATE_IDLE = 0,
  SHARD_STATE_RUNNING,
  SHARD_STATE_YIELDED,
  SHARD_STATE_COMPLETED,
  SHARD_STATE_ROLLED_BACK,
  SHARD_STATE_COMMITTED,
} bali_vm_shard_state_t;

typedef struct bali_vm_shard_t {
  struct bali_vm_shard_t	*parent;
  bali_vm_context_t		*context;
  bali_vm_frame_t		*frame;
  bali_bytecode_builder_t	*bc;
  bali_bump_arena_t		*bump;
  struct bali_vm_value_t	*exports;
  bsize_t			 export_capacity;
  bsize_t			 export_count;
  void				*stack;
  bsize_t                        stack_capacity;
  bsize_t			 pc;
  bali_vm_shard_type_t		 type;
  bali_vm_shard_state_t		 state;
} bali_vm_shard_t;

BALI_API void bali_vm_shard_init(bali_vm_shard_t *shard, bali_vm_context_t *context, bali_bytecode_builder_t *bc, bali_bump_arena_t *arena);
BALI_API void bali_vm_shard_setup(bali_vm_shard_t *shard, bali_vm_shard_t *parent, bali_vm_shard_type_t type, bsize_t pc);
BALI_API void bali_vm_shard_absorb_in_parent(bali_vm_shard_t *shard);
BALI_API void bali_vm_shard_execute(bali_vm_shard_t *shard);
BALI_API bali_vm_shard_state_t bali_vm_shard_state(bali_vm_shard_t *shard);
