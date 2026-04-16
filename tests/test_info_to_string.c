#include "test.h"

START_TEST(test_info_to_string) {
  char res[BUFSIZ] = {0};
  char* exp = "[INFO] test.c:9 log_fprint ";

  load_default_cfg();
  log_info info = {.filename = "test.c",
                   .funcname = "log_fprint",
                   .level = INFO,
                   .line = "9"};

  info_to_string(&info, res);
  ck_assert_str_eq(res, exp);
}
END_TEST

Suite* suite_info_to_string(void) {
  Suite* s = suite_create("suite_info_to_string");
  TCase* tc = tcase_create("Core");

  tcase_add_test(tc, test_info_to_string);

  suite_add_tcase(s, tc);
  return s;
}