#include "__bali_vm/value.h"
#include "bali_utilities.h"
#include <bali_vm.h>

void bali_vm_context_setup(bali_vm_context_t *context)
{
  BALI_DCHECK(context != nullptr);
  
  context->null = calloc(1, sizeof(bali_vm_value_t));
  BALI_RCHECK_NOT_NULL(context->null);
  context->undefined = calloc(1, sizeof(bali_vm_value_t));
  BALI_RCHECK_NOT_NULL(context->undefined);

  context->null->kind = BALI_VM_VALUE_NULL;
  context->undefined->kind = BALI_VM_VALUE_UNDEFINED;
  context->global_this = context->undefined;
}
