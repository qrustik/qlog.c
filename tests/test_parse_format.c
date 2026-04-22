#include "test.h"

START_TEST(test_parse_format_basic) {
  char* fmt = "[%l] %p:%n %f";
  char* exp_fmt = "[%s] %s:%s %s";

  parse_format(fmt);
  ck_assert_str_eq(cfg.fmt, exp_fmt);
}
END_TEST
// "pfln" l = 0 p = 1 n = 2 f = 3
START_TEST(test_parse_format_order) {
  char* fmt = "[%l] %p:%n %f";
  int exp_order[] = {0, 1, 2, 3};
  parse_format(fmt);
  ck_assert_mem_eq(exp_order, cfg.order, sizeof(exp_order));
}
END_TEST

START_TEST(test_parse_format_more_than_4_specs) {
  char* fmt = "[%l] %p:%f %f %n %f";
  char* exp_fmt = "[%s] %s:%s %f %s %f";
  int exp_order[] = {0, 1, 3, 2};

  parse_format(fmt);
  ck_assert_str_eq(cfg.fmt, exp_fmt);
  ck_assert_mem_eq(exp_order, cfg.order, sizeof(exp_order));
}
END_TEST

Suite* suite_parse_format(void) {
  Suite* s = suite_create("suite_parse_format");
  TCase* tc = tcase_create("Core");
  tcase_set_tags(tc, "suite_parse_format");

  tcase_add_test(tc, test_parse_format_basic);
  tcase_add_test(tc, test_parse_format_order);
  tcase_add_test(tc, test_parse_format_more_than_4_specs);

  suite_add_tcase(s, tc);
  return s;
}