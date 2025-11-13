#include "bali_utilities.h"
#include <bali_tokenizer.h>
#include <check.h>

#define SINGLE_TOKEN_TEST(name, src, token_kind)	\
  START_TEST(name)					\
  {							\
    bali_lexer_t lexer;					\
    bali_lexer_setup_from_cstring(&lexer, src);		\
    char output[256] = {0};				\
    bali_token_t *token;				\
    BALI_DCHECK(bali_lexer_next_token(&lexer, &token));	\
    bali_token_cstr(&lexer, token, output, 256);	\
    ck_assert_int_eq(token->kind, token_kind);		\
    ck_assert_str_eq(output, src);			\
  }							\
  END_TEST

SINGLE_TOKEN_TEST(can_lex_an_id, "myId", TK_IDENTIFIER)
SINGLE_TOKEN_TEST(can_lex_an_id_starting_with_underscore, "_myId", TK_IDENTIFIER)
SINGLE_TOKEN_TEST(can_lex_an_id_starting_with_underscores, "_my_Id", TK_IDENTIFIER)
SINGLE_TOKEN_TEST(can_lex_an_id_starting_with_numbers, "_my_Id123", TK_IDENTIFIER)
SINGLE_TOKEN_TEST(can_lex_a_private_id, "#_my_Id123", TK_PRIVATE_IDENTIFIER)

Suite *mylib_suite(void) {
    Suite *s = suite_create("bali_tokenizer");
    TCase *tc_core = tcase_create("tokenizer");
    tcase_add_test(tc_core, can_lex_an_id);
    tcase_add_test(tc_core, can_lex_an_id_starting_with_underscore);
    tcase_add_test(tc_core, can_lex_an_id_starting_with_underscores);
    tcase_add_test(tc_core, can_lex_an_id_starting_with_numbers);
    tcase_add_test(tc_core, can_lex_a_private_id);
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
