#include <stdio.h>
#include <time.h>

#include "test.h"

START_TEST(test_log_fprint_basic) {
  FILE* f = fopen(DEFAULT_FILEPATH, "a+");
  if (f == NULL) ck_abort_msg("Cannot open file %s", DEFAULT_FILEPATH);
  char res[MAX_BUF] = {0};
  char* exp = " [INFO] test.c:10 main ";
  char exp_time[MAX_FMT] = {0};
  log_info info = {
      .filename = "test.c", .level = INFO, .funcname = "main", .line = "10"};

  memset(&cfg, 0, sizeof(cfg));
  load_default_cfg();
  long cur = ftell(f);
  time_t t = time(NULL);
  strftime(exp_time, MAX_FMT, cfg.date_fmt, localtime(&t));
  log_fprint(f, &info);
  fseek(f, cur, SEEK_SET);
  if (fgets(res, MAX_BUF, f) == NULL) ck_abort_msg("fgets == NULL");
  fclose(f);
  remove(DEFAULT_FILEPATH);
  strcat(exp_time, exp);
  ck_assert_str_eq(res, exp_time);
}
END_TEST

Suite* suite_log_fprint(void) {
  Suite* s = suite_create("suite_log_fprint");
  TCase* tc = tcase_create("Core");

  tcase_add_test(tc, test_log_fprint_basic);

  suite_add_tcase(s, tc);
  return s;
}