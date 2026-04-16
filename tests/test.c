#include "test.h"

int main() {
  Suite* list[] = {suite_parse_format(),
                   suite_load_default_cfg(),
                   suite_log_fprint(),
                   suite_info_to_string(),
                   suite_log_msg(),
                   suite_LOG(),
                   NULL};
  for (int i = 0; list[i] != NULL; i++) {
    SRunner* sr = srunner_create(list[i]);
    srunner_set_fork_status(sr, CK_FORK_GETENV);
    srunner_run_all(sr, CK_NORMAL);
    srunner_free(sr);
  }
  return 0;
}