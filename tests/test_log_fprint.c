#include "test.h"

START_TEST(test_log_fprint_basic) {
  char res[BUFSIZ] = {0};
  char* timestamp = "00:12:53";
  char* exp = "00:12:53 [INFO] test.c:9 log_fprint";

  load_default_cfg();
  log_info info = {.filename = "test.c",
                   .funcname = "log_fprint",
                   .level = INFO,
                   .line = "9"};

  log_sprint(res, &info, timestamp);
  ck_assert_str_eq(res, exp);
}
END_TEST

Suite* suite_log_fprint(void) {
  Suite* s = suite_create("suite_log_fprint");
  TCase* tc = tcase_create("Core");

  tcase_add_test(tc, test_log_fprint_basic);

  suite_add_tcase(s, tc);
  return s;
}