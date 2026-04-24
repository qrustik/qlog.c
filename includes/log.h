#ifndef LOG_H
#define LOG_H

/**
 * @file log.h
 * @author qrustik
 * @brief Interface header for qlog.c
 * @date 2026-04-22
 */

// levels of log msg
typedef enum log_level {
  TRACE,
  DEBUG,
  INFO,
  WARNING,
  ERROR,
  FATAL,
  OFF,
  LEVELS_COUNT
} log_level_t;

// constant array sizes for config log variables
enum log_array_sizes { MAX_FMT = 64, MAX_BUF = 256, CNT_INFO_FIELDS = 4 };

#define LOG_TOSTR(a) LOG_STR(a)
#define LOG_STR(a) #a

// print log with TRACE level
#define LOGT(fmt, ...) LOG(TRACE, fmt, ##__VA_ARGS__)
// print log with DEBUG level
#define LOGD(fmt, ...) LOG(DEBUG, fmt, ##__VA_ARGS__)
// print log with INFO level
#define LOGI(fmt, ...) LOG(INFO, fmt, ##__VA_ARGS__)
// print log with WARNING level
#define LOGW(fmt, ...) LOG(WARNING, fmt, ##__VA_ARGS__)
// print log with ERROR level
#define LOGE(fmt, ...) LOG(ERROR, fmt, ##__VA_ARGS__)
// print log with FATAL level
#define LOGF(fmt, ...) LOG(FATAL, fmt, ##__VA_ARGS__)

// print log with meta info
#ifdef LOG_ON
#define LOG(LEVEL, fmt, ...)                       \
  log_msg((log_info){.filename = __FILE__,         \
                     .funcname = __func__,         \
                     .level = LEVEL,               \
                     .line = LOG_TOSTR(__LINE__)}, \
          fmt, ##__VA_ARGS__)
#else
#define LOG(LEVEL, fmt, ...)
#endif

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
  int wr_file;
  int wr_stderr;
  int log_on;
} log_cfg;

/**
 * @brief Print log message if calling first time initialized cfg global
 * structure
 * @param info that parameter contains meta-info of run-time
 * LEVEL-TIME-FILENAME-LINE-FUNCTION
 * @param fmt format string for log message
 * @return no return
 */
void log_msg(log_info info, const char* fmt, ...);

// global configuration structure that initialized with first call LOG()
extern log_cfg cfg;

#endif  // LOG_H