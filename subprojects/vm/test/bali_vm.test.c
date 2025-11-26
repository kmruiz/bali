#include <bali_utilities.h>
#include <bali_vm.h>
#include <check.h>

#include "__bali_vm/value.test.c"

int main(void) {
  int failed;
  Suite *s = suite_create("bali_vm");
  bali_vm_value_suite(s);
  
  SRunner *sr = srunner_create(s);
  srunner_run_all(sr, CK_ENV);
  failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (failed == 0) ? 0 : 1;
}
