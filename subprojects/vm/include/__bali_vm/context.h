#pragma once

#include <bali_utilities.h>

struct bali_vm_value_t;

typedef struct bali_vm_call_frame_t {
  struct bali_vm_frame_t	*parent;
  bsize_t			 slot_count;
  struct bali_vm_value_t	*slots;
} bali_vm_frame_t;

typedef struct bali_vm_context_t {
  struct bali_vm_value_t	*global_this;
  struct bali_vm_value_t        *undefined;
  struct bali_vm_value_t        *null;
} bali_vm_context_t;

BALI_API void bali_vm_context_setup(bali_vm_context_t *context);
