#include "log.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>

#include "helpers.h"
#include "macro.h"

log_cfg cfg = {0};

void log_msg(log_info info, const char* fmt, ...) {
  if (!cfg.is_load) {
    char* path = getenv(CFG_PATH_ENV);
    if (load_cfg(path) == EXIT_FAILURE) {
      load_default_cfg();
    }
    load_environ_cfg();
    cfg.is_load = 1;
  }
  if (!cfg.log_on) return;
  if (info.level < cfg.level) return;
  va_list args;
  va_start(args, fmt);
  if (cfg.wr_file) {
    FILE* f = fopen(cfg.filepath, "a+");
    if (f) {
      log_fprint(f, &info);
      vfprintf(f, fmt, args);
      fprintf(f, "\n");
      fclose(f);
    } else {
      fprintf(stderr, "Cannot open file %s\n", cfg.filepath);
    }
  }
  if (cfg.wr_stderr) {
    log_fprint(stderr, &info);
    vfprintf(stderr, fmt, args);
    fprintf(stderr, "\n");
  }
  va_end(args);
}