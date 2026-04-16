#include "test.h"

START_TEST(test_parse_format_basic) {
  char* fmt = "[%l] %p:%n %f";
  char* fmt_res = "[%s] %s:%d %s";

  parse_format(fmt);
  ck_assert_str_eq(cfg.fmt, fmt_res);
}
END_TEST
// "pfln"
START_TEST(test_parse_format_order) {
  char* fmt = "[%l] %p:%n %f";
  int res_order[] = {2, 0, 3, 1};

  parse_format(fmt);
  ck_assert_mem_eq(res_order, cfg.order, sizeof(res_order));
}
END_TEST

START_TEST(test_parse_format_more_than_4_specs) {
  char* fmt = "[%l] %p:%n %f %f %f";
  char* res_fmt = "[%s] %s:%d %s %f %n";
  int res_order[] = {2, 0, 3, 1};

  parse_format(fmt);
  ck_assert_str_eq(cfg.fmt, res_fmt);
  ck_assert_mem_eq(res_order, cfg.order, sizeof(res_order));
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