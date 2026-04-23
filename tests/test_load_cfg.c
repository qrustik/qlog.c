#include "test.h"

START_TEST(test_load_cfg_basic) {
  memset(&cfg, 0, sizeof(cfg));
  load_cfg(NULL);
  if (strcmp(cfg.fmt, "_%s_ [%s] %s:%s\n\t") != 0)
    ck_abort_msg("incorrect fmt: %s", cfg.fmt);
  if (strcmp(cfg.date_fmt, "[%H:%M:%S]") != 0)
    ck_abort_msg("incorrect date_fmt: %s", cfg.date_fmt);
  if (memcmp(cfg.order, (int[4]){0, 3, 1, 2}, sizeof(DEFAULT_ORDER)) != 0)
    ck_abort_msg("incorrect order");
  ck_assert_int_eq(cfg.level, DEBUG);
}
END_TEST

Suite* suite_load_cfg(void) {
  Suite* s = suite_create("suite_load_cfg");
  TCase* tc = tcase_create("Core");

  tcase_add_test(tc, test_load_cfg_basic);

  suite_add_tcase(s, tc);
  return s;
}