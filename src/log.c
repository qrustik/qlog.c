#include "log.h"

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/*
    ХОТЕЛКИ ОТ ЛОГЕРА:
    -настраивается формат вывода
    -настраивается формат времени
    -возможность настройки как внутри так и извне
    -как можно меньше оверхеда
    -кросс-платформ
    -*удобная настройка цвета
*/

const char* LOG_LEVELS[] = {"TRACE",   "DEBUG", "INFO",
                            "WARNING", "ERROR", "FATAL"};
log_cfg cfg = {0};

static int parse_format(const char* format) {
  size_t count = 0;
  size_t index = 0;
  char* specs = "pfln";
  char* fmt = cfg.fmt;
  while (*format) {
    if (*format == '%') {
      *fmt++ = *format++;
      index = strspn(specs, *format);
      if (index != 4) {
        cfg.order[count] = index;
      }
    } else {
    }
  }
  return 0;
}

static void load_cfg(const char* path) {
  FILE* f = fopen(path, "ro");
  char buf[MAX_BUF] = {0};
  while (fgets(buf, MAX_BUF, f)) {
    if (buf[0] == '\n' || buf[0] == '#') continue;
    const char* pos = strstr(buf, "fmt");
    if (pos) {
      pos += 4;
      parse_format(pos);
    }
  }
  fclose(f);
}

static void log_fprint(FILE* stream, const log_info* info) {
  (void)stream;
  (void)info;
}

void log_msg(log_info info, const char* fmt, ...) {
  if (info.level < cfg.level) return;
  va_list args;
  if (!cfg.is_load) {
    char* path = getenv("LOG_CFG_PATH");
    load_cfg(path);
  }
  va_start(args, fmt);
  log_fprint(stderr, &info);
  vfprintf(stderr, fmt, args);
  va_end(args);
}