#ifndef HELPERS_H
#define HELPERS_H

#include <stdio.h>

#include "log.h"

/**
 * @brief intitialize cfg global value with default values
 */
void load_default_cfg(void);

/**
 * @brief Open and read config file
 * @param path path of config file if path is NULL open DEFAULT_PATH
 * @return status code if 0 - success, 1 - something went wrong
 */
int load_cfg(const char* path);

/**
 * @brief parse config file to log config structure
 * @param stream of open file
 */
void parse_config_file(FILE* stream);

/**
 * @brief Parses the format of the string from the file and sets the index
 * of the log info structure depending on it specs: l - log level p - file
 * path n - line number f - func name
 * @param format format string for parsing
 * @return no return
 */
void parse_format(const char* format);

/**
 * @brief convert unescape sequence to escape
 * @param dest escaped string
 * @param str unescaped string
 */
void unescape_string(char* dest, const char* str);

/**
 * @brief check array have index value
 * @param array[] array size of CNT_INFO_FIELDS
 * @param index value that check
 * @return 1 if has index 0 - not
 */
int is_has_index(int array[CNT_INFO_FIELDS], int index);

/**
 * @brief convert string to log_level enum value
 * @param str string
 * @return log_level_t value
 */
log_level_t str_to_lvl(const char* str);

/**
 * @brief convert struct info to string
 * @return number of written chars
 */
int info_to_string(const log_info* info, char* str);

/**
 * @brief Print struct info into file stream
 * @param log_info meta info
 */
void log_fprint(FILE* stream, const log_info* info);

#endif  // HELPERS_H