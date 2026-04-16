#ifndef LOG_H
#define LOG_H

#include <stdio.h>

typedef enum log_level {
  TRACE,
  DEBUG,
  INFO,
  WARNING,
  ERROR,
  FATAL,
  OFF
} log_level_t;

// maximum length of format string
#define MAX_FMT 64
// maximum length of other strings like filepath
#define MAX_BUF 256
#define CNT_INFO_FIELDS 4

#define DEFAULT_CFG_PATH "qlog.config"
#define CFG_PATH_ENV "LOG_CFG_PATH"

#define DEFAULT_FORMAT "[%s] %s:%s %s\n"
#define DEFAULT_DATE_FORMAT "%H:%M:%S"
#define DEFAULT_FILEPATH __DATE__ ".log"
#define DEFAULT_ORDER ((int[4]){0, 1, 2, 3})
#define DEFAULT_LEVEL INFO

#define STR(a) #a

// print log with TRACE level
#define LOGT(fmt, ...) LOG(TRACE, fmt, __VA_ARGS__)
// print log with DEBUG level
#define LOGD(fmt, ...) LOG(DEBUG, fmt, __VA_ARGS__)
// print log with INFO level
#define LOGI(fmt, ...) LOG(INFO, fmt, __VA_ARGS__)
// print log with WARNING level
#define LOGW(fmt, ...) LOG(WARNING, fmt, __VA_ARGS__)
// print log with ERROR level
#define LOGE(fmt, ...) LOG(ERROR, fmt, __VA_ARGS__)
// print log with FATAL level
#define LOGF(fmt, ...) LOG(FATAL, fmt, __VA_ARGS__)

#define LOG(LEVEL, fmt, ...)                 \
  log_msg((log_info){.filename = __FILE__,   \
                     .funcname = __func__,   \
                     .level = LEVEL,         \
                     .line = STR(__LINE__)}, \
          fmt, __VA_ARGS__)

// Contains meta info about running programm
typedef struct log_info {
  log_level_t level;
  const char* filename;
  const char* line;
  const char* funcname;
} log_info;

// Contains logger options for output log info
typedef struct log_config {
  char fmt[MAX_FMT];
  char date_fmt[MAX_FMT];
  char filepath[MAX_BUF];
  log_level_t level;
  int is_load;
  int order[CNT_INFO_FIELDS];
} log_cfg;

void log_msg(log_info info, const char* fmt, ...);

// Remove from release version
void log_fprint(FILE* stream, const log_info* info);
void load_default_cfg();
int info_to_string(const log_info* info, char* str);
void parse_format(const char* format);

extern log_cfg cfg;

#endif  // LOG_H