#include <time.h>

#include "test.h"

START_TEST(test_log_msg_basic) {
  FILE* f = freopen(DEFAULT_FILEPATH, "a+", stderr);
  if (f == NULL) ck_abort_msg("Cannot open file %s", DEFAULT_FILEPATH);
  char res[MAX_BUF] = {0};
  char exp[MAX_BUF] =
      "Failed open file with name: qlog.config\n"
      "Loading default configuration for logger\n";
  char* infostr =
      " [INFO] test.c:9 main "
      "message from test\n";
  char exp_time[MAX_FMT] = {0};
  log_info info = {
      .filename = "test.c", .funcname = "main", .level = INFO, .line = "9"};
  long start = ftell(f);
  time_t t = time(NULL);
  log_msg(info, "message from test");
  strftime(exp_time, MAX_FMT, cfg.date_fmt, localtime(&t));
  long lenght = ftell(f) - start;
  fseek(f, start, SEEK_SET);
  fread(res, sizeof(char), lenght, f);
  fclose(f);
  remove(DEFAULT_FILEPATH);
  strcat(exp_time, infostr);
  strcat(exp, exp_time);
  ck_assert_str_eq(res, exp);
}
END_TEST

Suite* suite_log_msg(void) {
  Suite* s = suite_create("suite_log_msg");
  TCase* tc = tcase_create("Core");

  tcase_add_test(tc, test_log_msg_basic);

  suite_add_tcase(s, tc);
  return s;
}