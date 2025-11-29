#include <bali_utilities.h>
#include <bali_tokenizer.h>
#include <check.h>

#define SINGLE_TOKEN_TEST(name, src, token_kind)	\
  START_TEST(name)					\
  {							\
    bali_lexer_t lexer;					\
    bali_lexer_setup_from_cstring(&lexer, src);		\
    char output[256] = {0};				\
    bali_token_t *token;				\
    ck_assert(bali_lexer_next_token(&lexer, &token));	\
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
SINGLE_TOKEN_TEST(can_lex_a_decimal_i64_literal, "692834", TK_NUMERIC_LITERAL_BIGINT)
SINGLE_TOKEN_TEST(can_lex_a_decimal_f64_literal, "692834.234", TK_NUMERIC_LITERAL_RATIONAL)
SINGLE_TOKEN_TEST(can_lex_a_decimal_i64_literal_with_underscores, "692_834", TK_NUMERIC_LITERAL_BIGINT)
SINGLE_TOKEN_TEST(can_lex_a_decimal_f64_literal_with_underscores, "692_834.50_23", TK_NUMERIC_LITERAL_RATIONAL)
SINGLE_TOKEN_TEST(can_lex_a_dbl_quote_string, "\"abc\"", TK_DOUBLE_QUOTED_STRING)
SINGLE_TOKEN_TEST(can_lex_a_sgl_quote_string, "'abc'", TK_SINGLE_QUOTED_STRING)

SINGLE_TOKEN_TEST(can_lex_an_open_paren, "(", TK_PUNCT_OPEN_PARENTHESIS)
SINGLE_TOKEN_TEST(can_lex_a_closing_paren, ")", TK_PUNCT_CLOSING_PARENTHESIS)
SINGLE_TOKEN_TEST(can_lex_a_comma, ",", TK_PUNCT_COMMA)

START_TEST(can_lex_a_funcall)					
  {							
    bali_lexer_t lexer;					
    bali_lexer_setup_from_cstring(&lexer, "hello()");		
    char output[256] = {0};				
    bali_token_t *token;				
    ck_assert(bali_lexer_next_token(&lexer, &token));
    bali_token_cstr(&lexer, token, output, 256);	
    ck_assert_int_eq(token->kind, TK_IDENTIFIER);		
    ck_assert_str_eq(output, "hello");
    ck_assert(bali_lexer_next_token(&lexer, &token));
    ck_assert_int_eq(token->kind, TK_PUNCT_OPEN_PARENTHESIS);
    ck_assert(bali_lexer_next_token(&lexer, &token));
    ck_assert_int_eq(token->kind, TK_PUNCT_CLOSING_PARENTHESIS);
    ck_assert(!bali_lexer_next_token(&lexer, &token)); // EOF
    
  }							
  END_TEST

  START_TEST(can_lex_a_string_after_a_space)					
  {							
    bali_lexer_t lexer;					
    bali_lexer_setup_from_cstring(&lexer, " 'abc' ");		
    char output[256] = {0};				
    bali_token_t *token;	
    ck_assert(bali_lexer_next_token(&lexer, &token));
    bali_token_cstr(&lexer, token, output, 256);	
    ck_assert_int_eq(token->kind, TK_SINGLE_QUOTED_STRING);
    ck_assert_str_eq(output, "'abc'");
  }			
  END_TEST
  
Suite *bali_tokenizer_suite(void) {
    Suite *s = suite_create("bali_tokenizer");
    TCase *tc_core = tcase_create("tokenizer");
    tcase_add_test(tc_core, can_lex_an_id);
    tcase_add_test(tc_core, can_lex_an_id_starting_with_underscore);
    tcase_add_test(tc_core, can_lex_an_id_starting_with_underscores);
    tcase_add_test(tc_core, can_lex_an_id_starting_with_numbers);
    tcase_add_test(tc_core, can_lex_a_private_id);
    tcase_add_test(tc_core, can_lex_a_decimal_i64_literal);
    tcase_add_test(tc_core, can_lex_a_decimal_f64_literal);
    tcase_add_test(tc_core, can_lex_a_decimal_i64_literal_with_underscores);
    tcase_add_test(tc_core, can_lex_a_decimal_f64_literal_with_underscores);
    tcase_add_test(tc_core, can_lex_an_open_paren);
    tcase_add_test(tc_core, can_lex_a_closing_paren);
    tcase_add_test(tc_core, can_lex_a_comma);
    tcase_add_test(tc_core, can_lex_a_funcall);
    tcase_add_test(tc_core, can_lex_a_dbl_quote_string);
    tcase_add_test(tc_core, can_lex_a_sgl_quote_string);
    tcase_add_test(tc_core, can_lex_a_string_after_a_space);

    suite_add_tcase(s, tc_core);
    return s;
}


int main(void) {
    int failed;
    Suite *s = bali_tokenizer_suite();
    SRunner *sr = srunner_create(s);
    srunner_run_all(sr, CK_ENV);
    failed = srunner_ntests_failed(sr);
    srunner_free(sr);
    return (failed == 0) ? 0 : 1;
}
