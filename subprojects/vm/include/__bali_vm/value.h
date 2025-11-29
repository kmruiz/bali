#pragma once

#include "__bali_vm/context.h"
#include <bali_utilities.h>

typedef enum bali_vm_value_kind_t : uint8_t {
  BALI_VM_VALUE_BOOLEAN,
  BALI_VM_VALUE_I64,
  BALI_VM_VALUE_F64,
  BALI_VM_VALUE_STRING,
  BALI_VM_VALUE_OBJECT,
  BALI_VM_VALUE_FUNCTION,
  BALI_VM_VALUE_NULL,
  BALI_VM_VALUE_UNDEFINED,
} bali_vm_value_kind_t;

#define BALI_VM_STR_SSO_CAP ((int)(sizeof(char *) + sizeof(bsize_t) - 1))
static_assert(BALI_VM_STR_SSO_CAP < (1 << (sizeof(uint8_t) * 8)));

typedef struct bali_vm_string_t {
  union {
    struct {
      const char *ptr;
      bsize_t len;
    } heap;
    struct {
      char data[BALI_VM_STR_SSO_CAP];
      uint8_t len_and_tag;
    } sso;
  };
} bali_vm_string_t;

BALI_API void bali_vm_string_set(bali_vm_string_t *s, const char *input, bsize_t len);
BALI_API int bali_vm_string_cmp(const bali_vm_string_t *s1, const bali_vm_string_t *s2);
BALI_API bool bali_vm_string_cstr(const bali_vm_string_t *s, char *output, bsize_t capacity);
BALI_API bool bali_vm_string_in_heap(const bali_vm_string_t *s);
BALI_API bsize_t bali_vm_string_strlen(const bali_vm_string_t *s);

struct bali_vm_value_t;
struct bali_vm_call_frame_t;

typedef struct bali_vm_scope_t {
  bali_vm_context_t		*context;
  struct bali_vm_value_t	*vt[12];
  struct bali_vm_value_t	*this;
  void				*stack;
  void                          *stack_ptr;
} bali_vm_scope_t;

BALI_API struct bali_vm_value_t *bali_vm_scope_pop_value(bali_vm_scope_t *scope);

typedef struct bali_vm_key_value_pair_t {
  struct bali_vm_value_t	*key;
  struct bali_vm_value_t	*value;
} bali_vm_key_value_pair_t;

typedef struct bali_vm_packed_object_t {
  bali_vm_key_value_pair_t	*own_properties;
  uint16_t			 prop_len;
  uint16_t			 prop_capacity;
  struct bali_vm_value_t	*prototype;
} bali_vm_packed_object_t;

typedef struct bali_vm_function_t {
  struct bali_vm_value_t         *name;
  bool				 is_native;
  union {
    void (*native)(struct bali_vm_scope_t *);
    bsize_t			 bytecode_start;
  } fn;
  struct bali_vm_call_frame_t	*frame;
} bali_vm_function_t;

typedef struct bali_vm_value_t {
  bali_vm_value_kind_t		 kind;
  uint16_t			 refs;
  union {
    uint8_t                      boolean;
    int64_t			 i64;
    float64_t			 f64;
    bali_vm_string_t		 string;
    bali_vm_packed_object_t	 obj;
    bali_vm_function_t		 fn;
  };
} bali_vm_value_t;

BALI_API bool bali_vm_value_cstr(bali_vm_value_t *value, char *output, bsize_t capacity);
BALI_API bool bali_vm_value_is_trueish(bali_vm_value_t *value);
