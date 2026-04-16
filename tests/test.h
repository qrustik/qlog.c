#ifndef TEST_H
#define TEST_H

#include <check.h>

#include "log.h"

Suite* suite_parse_format(void);
Suite* suite_log_fprint(void);
Suite* suite_load_default_cfg(void);
Suite* suite_info_to_string(void);
Suite* suite_log_msg(void);
Suite* suite_LOG(void);

#endif  // TEST_H