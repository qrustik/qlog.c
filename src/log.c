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

// if  a is NULL returns b use preferly with variables
#define GET_OR(a, b)         \
  ({                         \
    __typeof__(a) _a = (a);  \
    (_a == NULL) ? (b) : _a; \
  })

// static const char* LOG_LEVELS[] = {"TRACE",   "DEBUG", "INFO",
//                                    "WARNING", "ERROR", "FATAL"};

// global configuration structure that initialized with first call LOG()
// function
log_cfg cfg = {0};

static int is_has_index(int array[CNT_INFO_FIELDS], int index);
//
static void init_cfg() { memset(cfg.order, -1, sizeof(cfg.order)); }

/**
 * @brief Parses the format of the string from the file and sets the index of
 * the log info structure depending on it
 * specs:
 * p - file path
 * f - func name
 * l - log level
 * n - line number
 * @param format format string for parsing
 * @return no return
 */
void parse_format(const char* format) {
  size_t index = 0;
  char* specs = "pfln";
  char* specs_fmt = "sssd";
  init_cfg();
  char* fmt = cfg.fmt;
  for (size_t i = 0; *format;) {
    if (*format == '%' && i < CNT_INFO_FIELDS) {
      *fmt++ = *format++;
      index = strchr(specs, *format) - specs;
      if (index < CNT_INFO_FIELDS && !is_has_index(cfg.order, index)) {
        cfg.order[i++] = index;
        *fmt++ = specs_fmt[index];
      } else {
        *fmt++ = *format;
      }
    } else {
      *fmt++ = *format;
    }
    format++;
  }
}

// check array has index
static int is_has_index(int array[CNT_INFO_FIELDS], int index) {
  int ret = 0;
  for (int i = 0; i < CNT_INFO_FIELDS; i++) {
    if (array[i] == index) {
      ret = 1;
      break;
    }
  }
  return ret;
}

/**
 * @brief Open and read config file
 * @param path path of config file if path is NULL open DEFAULT_PATH
 * @return status code if 0 - success, 1 - something went wrong
 */
int load_cfg(const char* path) {
  FILE* f = fopen(GET_OR(path, DEFAULT_CFG_PATH), "ro");
  if (f == NULL) {
    fprintf(stderr, "Failed open file with name: %s\n",
            GET_OR(path, DEFAULT_CFG_PATH));
    return EXIT_FAILURE;
  }
  char buf[MAX_BUF] = {0};
  while (fgets(buf, MAX_BUF, f)) {
    if (buf[0] == '\n' || buf[0] == '#') continue;
    const char* pos = strstr(buf, "fmt");
    if (pos) {
      pos += strlen("fmt");
      parse_format(pos);
    }
  }
  fclose(f);
  return EXIT_SUCCESS;
}

void log_fprint(FILE* stream, const log_info* info) {
  (void)stream;
  (void)info;
}
/**
 * @brief Print log message if calling first time initialized cfg global
 * structure
 * @param info that parameter contains meta-info of run-time
 * LEVEL-TIME-FILENAME-LINE-FUNCTION
 * @param fmt format string for log message
 * @return no return
 */
void log_msg(log_info info, const char* fmt, ...) {
  if (!cfg.is_load) {
    char* path = getenv(CFG_PATH_ENV);
    if (load_cfg(path) == EXIT_FAILURE) {
      load_default_cfg();
    }
  }
  if (info.level < cfg.level) return;
  va_list args;
  va_start(args, fmt);
  log_fprint(stderr, &info);
  vfprintf(stderr, fmt, args);
  va_end(args);
}

void load_default_cfg() {
  fprintf(stderr, "Loading default configuration for logger\n");
  strcpy(cfg.fmt, DEFAULT_FORMAT);
  strcpy(cfg.date_fmt, DEFAULT_DATE_FORMAT);
  cfg.level = INFO;
  memcpy(cfg.order, DEFAULT_ORDER, sizeof(cfg.order));
  strcpy(cfg.filepath, DEFAULT_FILEPATH);
  cfg.is_load = 1;
}