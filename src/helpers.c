#include "helpers.h"

#include <string.h>
#include <time.h>

#include "macro.h"

static const char* LOG_LEVELS[] = {"TRACE",   "DEBUG", "INFO",
                                   "WARNING", "ERROR", "FATAL"};

void load_default_cfg(void) {
  fprintf(stderr, "Loading default configuration for logger\n");
  strcpy(cfg.fmt, DEFAULT_FORMAT);
  strcpy(cfg.date_fmt, DEFAULT_DATE_FORMAT);
  cfg.level = INFO;
  memmove(cfg.order, DEFAULT_ORDER, sizeof(cfg.order));
  strcpy(cfg.filepath, DEFAULT_FILEPATH);
  cfg.wr_file = 0;
  cfg.wr_stderr = 1;
  cfg.log_on = 1;
}

void parse_format(const char* format) {
  size_t index = 0;
  const char* specs = "lpnf";
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

void unescape_string(char* dest, const char* str) {
  const char* sequence = "ntrvf0\\\"\'\0";
  const char* replace = "\n\t\r\v\b\f\0\\\"\'";
  size_t len_seq = strlen(sequence);
  size_t index = 0;
  while (*str) {
    if (*str == '\\') {
      str++;
      index = strchr(sequence, *str) - sequence;
      if (index < len_seq) {
        if (sequence[index] == '\0')
          str--;
        else
          *dest++ = replace[index];
      } else {
        *dest++ = *str;
      }
    } else {
      *dest++ = *str;
    }
    str++;
  }
}

int is_has_index(int array[CNT_INFO_FIELDS], int index) {
  int ret = 0;
  for (int i = 0; i < CNT_INFO_FIELDS; i++) {
    if (array[i] == index) {
      ret = 1;
      break;
    }
  }
  return ret;
}

log_level_t str_to_lvl(const char* str) {
  log_level_t index = 0;
  for (; index < LEVELS_COUNT; index++) {
    if (strcmp(LOG_LEVELS[index], str) == 0) return index;
  }
  return index;
}

void parse_config_file(FILE* stream) {
  char buf[MAX_BUF] = {0};
  while (fgets(buf, MAX_BUF, stream) != NULL) {
    if (buf[0] == '#' || buf[0] == ';') continue;
    char* token = strtok(buf, " =");
#define MATCH(a, b) (strcmp(a, b) == 0)
    do {
      if (MATCH(token, "FMT")) {
        token = strtok(NULL, "\"");
        char format[MAX_FMT] = {0};
        unescape_string(format, token);
        parse_format(format);
      } else if (MATCH(token, "DATE_FMT")) {
        token = strtok(NULL, "\"");
        strcpy(cfg.date_fmt, token);
      } else if (MATCH(token, "FILEPATH")) {
        token = strtok(NULL, "\"");
        strcpy(cfg.filepath, token);
      } else if (MATCH(token, "WR_STDERR")) {
        token = strtok(NULL, "\"");
        cfg.wr_stderr = strcmp("1", token) == 0;
      } else if (MATCH(token, "WR_FILE")) {
        token = strtok(NULL, "\"");
        cfg.wr_file = strcmp("1", token) == 0;
      } else if (MATCH(token, "LEVEL")) {
        token = strtok(NULL, "\"");
        int level = str_to_lvl(token);
        if (level < LEVELS_COUNT && level > 0) {
          cfg.level = level;
        } else {
          cfg.level = DEFAULT_LEVEL;
          fprintf(stderr, "INCORRECT LEVEL VALUE FROM CONFIG FILE");
        }
      }
    } while ((token = strtok(NULL, " =\n")) != NULL);
#undef MATCH
  }
}

int load_cfg(const char* path) {
  FILE* f = fopen(GET_OR(path, DEFAULT_CFG_PATH), "rb");
  if (f == NULL) {
    fprintf(stderr, "Failed open file with name: %s\n",
            GET_OR(path, DEFAULT_CFG_PATH));
    return EXIT_FAILURE;
  }
  parse_config_file(f);
  fclose(f);
  return EXIT_SUCCESS;
}

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

void log_fprint(FILE* stream, const log_info* info) {
  char timestamp[MAX_BUF] = {0};
  time_t t = time(NULL);
  strftime(timestamp, MAX_BUF, cfg.date_fmt, localtime(&t));
  fprintf(stream, "%s ", timestamp);
  char str[MAX_BUF] = {0};
  info_to_string(info, str);
  fprintf(stream, "%s", str);
}