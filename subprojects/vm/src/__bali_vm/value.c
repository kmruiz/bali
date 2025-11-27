#include "__bali_vm/value.h"
#include <bali_vm.h>
#include <bali_utilities.h>
#include <stdint.h>
#include <string.h>

#define BALI_VM_STR_TAG_HEAP   0x80u
#define BALI_VM_STR_SSO_LEN_MASK 0x7Fu
#define BALI_VM_STR_MAX_LEN    ((uint64_t)0x00FFFFFFFFFFFFFFull)

void bali_vm_string_set(bali_vm_string_t *s, const char *output, bsize_t len)
{
  BALI_DCHECK(s != nullptr);
  BALI_DCHECK(output != nullptr);
  BALI_DCHECK_BSIZE_BOUNDS(len);
  
  if (len <= BALI_VM_STR_SSO_CAP) {
    s->sso.len_and_tag = (uint8_t) len;
    memcpy(s->sso.data, output, len);
    if (len < BALI_VM_STR_SSO_CAP) {
      s->sso.data[len] = 0;
    }
  } else {
    s->heap.ptr = output;
    s->heap.len = len;
    s->sso.len_and_tag = BALI_VM_STR_TAG_HEAP;
  }
}

int bali_vm_string_cmp(const bali_vm_string_t *s1, const bali_vm_string_t *s2)
{
  BALI_DCHECK(s1 != nullptr);
  BALI_DCHECK(s2 != nullptr);

  if (s1 == s2) {
    return 0;
  }

  bsize_t len1 = bali_vm_string_strlen(s1);
  bsize_t len2 = bali_vm_string_strlen(s2);

  if (len1 < len2) return -1;
  if (len1 > len2) return  1;

  const char *p1 = bali_vm_string_in_heap(s1) ? s1->heap.ptr : s1->sso.data;
  const char *p2 = bali_vm_string_in_heap(s2) ? s2->heap.ptr : s2->sso.data;

  BALI_DCHECK_BSIZE_BOUNDS(len1);
  int r = memcmp(p1, p2, (size_t)len1);
  if (r < 0) return -1;
  if (r > 0) return  1;
  return 0;
}

bool bali_vm_string_cstr(const bali_vm_string_t *s, char *output, bsize_t capacity)
{
  BALI_DCHECK(s != nullptr);
  BALI_DCHECK(output != nullptr);
  BALI_DCHECK_BSIZE_BOUNDS(capacity);

  const bsize_t len = bali_vm_string_strlen(s);
  if (len + 1 > capacity) {
    return false;
  }
  
  const char *data = bali_vm_string_in_heap(s) ? s->heap.ptr : s->sso.data;
  memcpy(output, data, len);
  output[len] = 0;
  
  return true;
}

bool bali_vm_string_in_heap(const bali_vm_string_t *s)
{
  BALI_DCHECK(s != nullptr);
  return (s->sso.len_and_tag & BALI_VM_STR_TAG_HEAP) != 0;
}

bsize_t bali_vm_string_strlen(const bali_vm_string_t *s)
{
  BALI_DCHECK(s != nullptr);
  if (bali_vm_string_in_heap(s)) {
    return s->heap.len & BALI_VM_STR_MAX_LEN;
  }
  
  return s->sso.len_and_tag & (~BALI_VM_STR_TAG_HEAP);
}


bool bali_vm_value_cstr(bali_vm_value_t *value, char *output, bsize_t capacity)
{
  BALI_DCHECK(value != nullptr);
  BALI_DCHECK(output != nullptr);
  BALI_DCHECK_BSIZE_BOUNDS(capacity);

  switch (value->kind) {
  case BALI_VM_VALUE_I64:
    return snprintf(output, capacity, "%ld", value->i64) < capacity;
    break;
  case BALI_VM_VALUE_F64:
    return snprintf(output, capacity, "%f", value->f64) < capacity;    
    break;
  case BALI_VM_VALUE_STRING:
    return bali_vm_string_cstr(value->string, output, capacity);
    break;
  case BALI_VM_VALUE_OBJECT:
    return false;
    break;
  case BALI_VM_VALUE_FUNCTION:
    return false;
    break;
  case BALI_VM_VALUE_NULL:
    return snprintf(output, capacity, "null") < capacity;
    break;
  case BALI_VM_VALUE_UNDEFINED:
    return snprintf(output, capacity, "undefined") < capacity;
    break;
  }
}
