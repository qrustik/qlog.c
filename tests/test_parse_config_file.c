#include "test.h"

void parse_config_file(FILE* stream);
/*
FMT="_%l_ [%f] %p:%n\n\t"
DATE_FMT="[%H:%M:%S]"
LEVEL="DEBUG"
WR_FILE="1"
WR_STDERR="1"
FILEPATH="test.log"
*/

START_TEST(test_parse_config_file_basic) {
  log_cfg exp = {.date_fmt = "[%H:%M:%S]",
                 .filepath = "test.log",
                 .fmt = "_%s_ [%s] %s:%s\n\t",
                 .level = DEBUG,
                 .wr_file = 1,
                 .wr_stderr = 1};
  FILE* f = fopen("qlog.config", "rb");
  if (!f) ck_abort_msg("filestream is NULL");
  parse_config_file(f);

  ck_assert_str_eq(exp.date_fmt, cfg.date_fmt);
  ck_assert_str_eq(exp.fmt, cfg.fmt);
  ck_assert_str_eq(exp.filepath, cfg.filepath);
  ck_assert_int_eq(exp.level, cfg.level);
  ck_assert_int_eq(exp.wr_file, cfg.wr_file);
  ck_assert_int_eq(exp.wr_stderr, cfg.wr_stderr);
}
END_TEST

Suite* suite_parse_config_file(void) {
  Suite* s = suite_create("suite_parse_config_file");
  TCase* tc = tcase_create("Core");

  tcase_add_test(tc, test_parse_config_file_basic);

  suite_add_tcase(s, tc);
  return s;
}