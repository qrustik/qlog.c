#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "log.h"

/*
    ХОТЕЛКИ ОТ ЛОГЕРА:
    -настраивается формат вывода
    -настраивается формат времени
    -возможность настройки как внутри так и извне
    -как можно меньше оверхеда
    -кросс-платформ
    -*удобная настройка цвета
*/


logger_t logger = {0};

static lvl getenv_level(const char* env) {
    lvl level = OFF;
    char *LEVEL = getenv(env);

    if (strcmp(LEVEL, "INFO"))
        level = INFO;
    else if (strcmp(LEVEL, "DEBUG"))
        level = DEBUG;
    else if (strcmp(LEVEL, "WARN"))
        level = WARN;
    return level;
}

static void init(const char* fmt, lvl level) {
    if (level == OFF) {
        logger.level = getenv_level("LOG_LEVEL");
    }else {
        logger.level = level;
    }

    if(fmt == NULL){
        logger.fmt = "[%s][%s] %s:%d=>%s msg:%s";
    }else{
        logger.fmt = fmt;
    }
}

void log(const char* fmt, ...){
    va_list args;
    if (!logger.is_init) init(NULL, OFF);
    char buf[16];
    time_t t = time(NULL);
    
    buf[strftime(buf, 16, "%Y-%m-%d %H:%M:%S", localtime(&t))] = 0;
    // fprintf(logger.fmt, logger.level, buf, );
    vfprintf(stderr, fmt, args);
    va_end(args);
}