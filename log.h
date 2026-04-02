#ifndef LOG_H
#define LOG_H

#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

typedef enum{DEBUG = 0, INFO = 1, WARN = 2, OFF = -1}lvl;

typedef struct {
    char* fmt;
    char* level;
    int is_init;
}logger_t;

extern logger_t logger;

void log(const char* fmt, ...);

#endif // LOG_H