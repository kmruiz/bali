#include "bali_utilities.h"
#include <bali_tokenizer.h>
#include <check.h>

START_TEST(can_lex_a_single_identifier)
{
  bali_lexer_t lexer;
  BALI_DCHECK(bali_lexer_setup_from_cstring(&lexer, "myId"));
  
}
END_TEST

Suite *mylib_suite(void) {
    Suite *s = suite_create("bali_tokenizer");
    TCase *tc_core = tcase_create("placeholder");
    tcase_add_test(tc_core, placeholder);
    suite_add_tcase(s, tc_core);
    return s;
}


int main(void) {
    int failed;
    Suite *s = mylib_suite();
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_ENV);
    failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (failed == 0) ? 0 : 1;
}
