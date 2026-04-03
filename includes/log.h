#ifndef LOG_H
#define LOG_H

typedef enum { TRACE, DEBUG, INFO, WARNING, ERROR, FATAL, OFF } log_level_t;

extern const char* LOG_LEVELS[];

#define MAX_FMT 16
#define MAX_BUF 256

#define LOGT(fmt, ...) LOG(TRACE, fmt, __VA_ARGS__)
#define LOGD(fmt, ...) LOG(DEBUG, fmt, __VA_ARGS__)
#define LOGI(fmt, ...) LOG(INFO, fmt, __VA_ARGS__)
#define LOGW(fmt, ...) LOG(WARNING, fmt, __VA_ARGS__)
#define LOGE(fmt, ...) LOG(ERROR, fmt, __VA_ARGS__)
#define LOGF(fmt, ...) LOG(FATAL, fmt, __VA_ARGS__)

#define LOG(LEVEL, fmt, ...)               \
  log_msg((log_info){.filename = __FILE__, \
                     .funcname = __func__, \
                     .level = LEVEL,       \
                     .line = __LINE__},    \
          fmt, __VA_ARGS__)

typedef struct {
  const char* filename;
  const char* funcname;
  log_level_t level;
  int line;
} log_info;

typedef struct {
  char fmt[MAX_FMT];
  char date_fmt[MAX_FMT];
  char cfg_filename[MAX_BUF];
  log_level_t level;
  int is_load;
  int order[4];
} log_cfg;

extern log_cfg cfg;

void log_msg(log_info info, const char* fmt, ...);

#endif  // LOG_H