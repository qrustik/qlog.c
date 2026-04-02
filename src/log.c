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

static int parse_format(const char* buf, const char* delim){
	(void)buf;
	(void)delim;
	return 0;
}


static void load_cfg(const char* path) {
  FILE* f = fopen(path, "ro");
  char buf[MAX_BUF] = {0};
  char* delim = "=";
  while (fgets(buf, MAX_BUF, f)) {
    if (buf[0] == '\n' || buf[0] == '#') continue;
		if (!parse_format(buf, delim)) LOGW("qlog cant parse format string: %s\tfrom file: %s\n", buf, path);
  }
  fclose(f);
}

static void log_fprint(FILE* stream, const log_info* info){
	(void)stream;
	(void)info;
}


void log_msg(log_info info, const char* fmt, ...) {
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