#include <__bali_vm/value.h>
#include <check.h>

START_TEST(a_small_string_fits_in_sso)					
{
  bali_vm_string_t value;
  bali_vm_string_set(&value, "hello!", 6);

  ck_assert(!bali_vm_string_in_heap(&value));
  ck_assert_int_eq(bali_vm_string_strlen(&value), 6);

  char output[256];
  bali_vm_string_cstr(&value, output, 256);
  ck_assert_str_eq(output, "hello!");
}
END_TEST

START_TEST(a_big_string_stays_in_heap_mode)					
{
  const char *str = "hello world this is longer than 16 bytes!";
  const bsize_t len = strlen(str);
  
  bali_vm_string_t value;
  bali_vm_string_set(&value, str, len);

  ck_assert(bali_vm_string_in_heap(&value));
  ck_assert_int_eq(bali_vm_string_strlen(&value), len);
  ck_assert_ptr_eq(value.heap.ptr, str); // it's important to use the same ptr ref
  
  char output[256];
  bali_vm_string_cstr(&value, output, 256);
  ck_assert_str_eq(output, str);
}
END_TEST

START_TEST(can_compare_two_sso_strings)					
{
  const char *str1 = "hello!";
  const char *str2 = "world!";
  
  const bsize_t len1 = strlen(str1);
  const bsize_t len2 = strlen(str2);
  
  bali_vm_string_t val1;
  bali_vm_string_t val2;
  
  bali_vm_string_set(&val1, str1, len1);
  bali_vm_string_set(&val2, str2, len2);

  ck_assert_int_eq(bali_vm_string_cmp(&val1, &val1), 0);
  ck_assert_int_eq(bali_vm_string_cmp(&val1, &val2), -1);
  ck_assert_int_eq(bali_vm_string_cmp(&val2, &val1), 1);  
}
END_TEST

START_TEST(can_compare_two_non_sso_strings)					
{
  const char *str1 = "helloooooooooooooooooooooo!";
  const char *str2 = "worldddddddddddddddddddddd!";
  
  const bsize_t len1 = strlen(str1);
  const bsize_t len2 = strlen(str2);
  
  bali_vm_string_t val1;
  bali_vm_string_t val2;
  
  bali_vm_string_set(&val1, str1, len1);
  bali_vm_string_set(&val2, str2, len2);

  ck_assert_int_eq(bali_vm_string_cmp(&val1, &val1), 0);
  ck_assert_int_eq(bali_vm_string_cmp(&val1, &val2), -1);
  ck_assert_int_eq(bali_vm_string_cmp(&val2, &val1), 1);  
}
END_TEST

Suite *bali_vm_value_suite(Suite *s) {
  TCase *tc_core = tcase_create("string");
  tcase_add_test(tc_core, a_small_string_fits_in_sso);
  tcase_add_test(tc_core, can_compare_two_sso_strings);
  tcase_add_test(tc_core, a_big_string_stays_in_heap_mode);
  tcase_add_test(tc_core, can_compare_two_non_sso_strings);
  suite_add_tcase(s, tc_core);
  return s;
}
