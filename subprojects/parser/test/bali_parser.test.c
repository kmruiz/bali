#include <bali_utilities.h>
#include <bali_parser.h>
#include <check.h>

int main(void) {
  int failed;
  Suite *s = suite_create("bali_parser");
  
  SRunner *sr = srunner_create(s);
  srunner_run_all(sr, CK_ENV);
  failed = srunner_ntests_failed(sr);
  srunner_free(sr);
  return (failed == 0) ? 0 : 1;
}
