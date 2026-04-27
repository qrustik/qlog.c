#include "test.h"

START_TEST(test_LOG_BASIC) {
  memset(&cfg, 0, sizeof(cfg));
  FILE* f = freopen(DEFAULT_FILEPATH, "a+", stderr);
  if (!f) ck_abort_msg("CANNOT OPEN FILE %s", DEFAULT_FILEPATH);
  char res[MAX_BUF] = {0};
  long start = ftell(f);
  LOG(INFO, "MESSAGE FOR LOGS");
  long length = ftell(f) - start;
  fseek(f, start, SEEK_SET);
  fread(res, sizeof(char), length, f);
  fclose(f);
  remove(DEFAULT_FILEPATH);
  f = freopen("hello.log", "a", stderr);
  LOGI("HELLO LOG\n");
  fclose(f);

  LOGI("HELLO FILE!");

  ck_abort_msg(res);
}
END_TEST

Suite* suite_LOG(void) {
  Suite* s = suite_create("suite_LOG");
  TCase* tc = tcase_create("Core");

  tcase_add_test(tc, test_LOG_BASIC);

  suite_add_tcase(s, tc);
  return s;
}