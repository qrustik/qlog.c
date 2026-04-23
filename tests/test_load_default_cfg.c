#include "test.h"

START_TEST(test_load_default_cfg_basic) {
  memset(&cfg, 0, sizeof(cfg));
  load_default_cfg();
  ck_assert_str_eq(cfg.fmt, DEFAULT_FORMAT);
  ck_assert_str_eq(cfg.date_fmt, DEFAULT_DATE_FORMAT);
  ck_assert_str_eq(cfg.filepath, DEFAULT_FILEPATH);
  ck_assert_int_eq(cfg.level, DEFAULT_LEVEL);
  ck_assert_mem_eq(cfg.order, DEFAULT_ORDER, sizeof(DEFAULT_ORDER));
}
END_TEST

Suite* suite_load_default_cfg(void) {
  Suite* s = suite_create("suite_load_default_cfg");
  TCase* tc = tcase_create("Core");

  tcase_add_test(tc, test_load_default_cfg_basic);

  suite_add_tcase(s, tc);
  return s;
}