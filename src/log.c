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

static const char* LOG_LEVELS[] = {"TRACE",   "DEBUG", "INFO",
                                   "WARNING", "ERROR", "FATAL"};

// global configuration structure that initialized with first call LOG()
log_cfg cfg = {0};
static int LOG_ON = 1;

static int is_has_index(int array[CNT_INFO_FIELDS], int index);

/**
 * @brief Parses the format of the string from the file and sets the index of
 * the log info structure depending on it
 * specs:
 * l - log level
 * p - file path
 * n - line number
 * f - func name
 * @param format format string for parsing
 * @return no return
 */
void parse_format(const char* format) {
  size_t index = 0;
  char* specs = "lpnf";
  memset(cfg.order, -1, sizeof(cfg.order));
  char* fmt = cfg.fmt;
  for (size_t i = 0; *format;) {
    if (*format == '%' && i < CNT_INFO_FIELDS) {
      *fmt++ = *format++;
      index = strchr(specs, *format) - specs;
      if (index < CNT_INFO_FIELDS && !is_has_index(cfg.order, index)) {
        cfg.order[i++] = index;
        *fmt++ = 's';
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
 * @brief parse config file to log config structure
 * @param stream of open file
 */
void parse_config_file(FILE* stream) {
  char buf[MAX_BUF] = {0};
  while (fgets(buf, MAX_BUF, stream) != NULL) {
    if (buf[0] == "#" || buf[0] == ";") continue;
    char* token = strtok(buf, " =");
#define MATCH(a, b) (strcmp(a, b) == 0)
    do {
      if (MATCH(token, "FMT")) {
        token = strtok(NULL, " \"");
        strcpy(cfg.fmt, token);
      } else if (MATCH(token, "DATE_FMT")) {
        token = strtok(NULL, " \"");
        strcpy(cfg.date_fmt, token);
      } else if (MATCH(token, "FILEPATH")) {
        token = strtok(NULL, " \"");
        strcpy(cfg.filepath, token);
      } else if (MATCH(token, "WR_STDERR")) {
        token = strtok(NULL, " \"");
        cfg.wr_stderr = atoi(token) ? 1 : 0;
      } else if (MATCH(token, "WR_FILE")) {
        token = strtok(NULL, " \"");
        cfg.wr_file = atoi(token) ? 1 : 0;
      } else if (MATCH(token, "LEVEL")) {
        token = strtok(NULL, " \"");
        int level = atoi(token);
        if (level < LEVELS_COUNT && level > 0) {
          cfg.level = level;
        } else {
          cfg.level = DEFAULT_LEVEL;
          fprintf(stderr, "INCORRECT LEVEL VALUE FROM CONFIG FILE");
        }
      }
    } while ((token = strtok(NULL, " =")) != NULL);
#undef MATCH
  }
}

/**
 * @brief Open and read config file
 * @param path path of config file if path is NULL open DEFAULT_PATH
 * @return status code if 0 - success, 1 - something went wrong
 */
int load_cfg(const char* path) {
  FILE* f = fopen(GET_OR(path, DEFAULT_CFG_PATH), "r");
  if (f == NULL) {
    fprintf(stderr, "Failed open file with name: %s\n",
            GET_OR(path, DEFAULT_CFG_PATH));
    return EXIT_FAILURE;
  }
  fclose(f);
  return EXIT_SUCCESS;
}

/**
 * @brief convert struct info to string
 * @return number of written chars
 */
int info_to_string(const log_info* info, char* str) {
  const char* msg_info[CNT_INFO_FIELDS] = {0};
  for (size_t i = 0; i < CNT_INFO_FIELDS; i++) {
    if (cfg.order[i] == 0)
      msg_info[i] = LOG_LEVELS[info->level];
    else if (cfg.order[i] == 1)
      msg_info[i] = info->filename;
    else if (cfg.order[i] == 2)
      msg_info[i] = info->line;
    else if (cfg.order[i] == 3)
      msg_info[i] = info->funcname;
  }
  return sprintf(str, cfg.fmt, msg_info[0], msg_info[1], msg_info[2],
                 msg_info[3]);
}

/**
 * @brief Print struct info into file stream
 */
void log_fprint(FILE* stream, const log_info* info) {
  char timestamp[MAX_BUF] = {0};
  time_t t = time(NULL);
  strftime(timestamp, MAX_BUF, cfg.date_fmt, localtime(&t));
  fprintf(stream, "%s ", timestamp);
  char str[MAX_BUF] = {0};
  info_to_string(info, str);
  fprintf(stream, "%s", str);
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
  if (!LOG_ON) return;
  if (!cfg.is_load) {
    LOG_ON = atoi(GET_OR(getenv("LOG_ON"), "1"));
    if (!LOG_ON) return;
    char* path = getenv(CFG_PATH_ENV);
    if (load_cfg(path) == EXIT_FAILURE) {
      load_default_cfg();
    }
  }
  if (info.level < cfg.level) return;
  va_list args;
  va_start(args, fmt);
  if (cfg.wr_file) {
    FILE* f = fopen(cfg.filepath, "a");
    if (f) {
      log_fprint(stderr, &info);
      vfprintf(stderr, fmt, args);
      fprintf(stderr, "\n");
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

// Fills the fields of the structure with default values
void load_default_cfg(void) {
  fprintf(stderr, "Loading default configuration for logger\n");
  strcpy(cfg.fmt, DEFAULT_FORMAT);
  strcpy(cfg.date_fmt, DEFAULT_DATE_FORMAT);
  cfg.level = INFO;
  memcpy(cfg.order, DEFAULT_ORDER, sizeof(cfg.order));
  strcpy(cfg.filepath, DEFAULT_FILEPATH);
  cfg.is_load = 1;
}