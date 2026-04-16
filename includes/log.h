#ifndef LOG_H
#define LOG_H

typedef enum { TRACE, DEBUG, INFO, WARNING, ERROR, FATAL, OFF } log_level_t;
// maximum length of format string
#define MAX_FMT 16
// maximum length of other strings like filepath
#define MAX_BUF 256

#define DEFAULT_PATH "qlog.config"
#define CFG_PATH_ENV "LOG_CFG_PATH"

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

#define LOG(LEVEL, fmt, ...)               \
  log_msg((log_info){.filename = __FILE__, \
                     .funcname = __func__, \
                     .level = LEVEL,       \
                     .line = __LINE__},    \
          fmt, __VA_ARGS__)

// Contains meta info about running programm
typedef struct {
  const char* filename;
  const char* funcname;
  log_level_t level;
  int line;
} log_info;

// Contains logger options for output log info
typedef struct {
  char fmt[MAX_FMT];
  char date_fmt[MAX_FMT];
  char cfg_filename[MAX_BUF];
  log_level_t level;
  int is_load;
  int order[4];
} log_cfg;

void log_msg(log_info info, const char* fmt, ...);

// Remove from release version
void parse_format(const char* format);

extern log_cfg cfg;

#endif  // LOG_H