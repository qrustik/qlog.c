#ifndef MACRO_H
#define MACRO_H

// default cfg values
#define DEFAULT_CFG_PATH "qlog.config"
#define CFG_PATH_ENV "LOG_CFG_PATH"
#define DEFAULT_FORMAT "[%s] %s:%s %s "
#define DEFAULT_DATE_FORMAT "%H:%M:%S"
#define DEFAULT_FILEPATH __DATE__ ".log"
#define DEFAULT_ORDER ((int[4]){0, 1, 2, 3})
#define DEFAULT_LEVEL INFO

#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1

// if  a is NULL returns b use preferly with variables
#define GET_OR(a, b)         \
  ({                         \
    __typeof__(a) _a = (a);  \
    (_a == NULL) ? (b) : _a; \
  })

#endif  // MACRO_H