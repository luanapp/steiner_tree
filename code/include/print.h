

#ifndef _PRINT_H_
#define _PRINT_H_

#include <stdio.h>
#include <stdlib.h>
#include <stdarg.h>
#include <string.h>

#define PRINT_INFO  0
#define PRINT_ERROR 1
#define PRINT_WARN  2
#define PRINT_DEBUG 3

/* *
 * Disable warining for the variadic-macro - use of ##__VA_ARGS__
 * This disable only for this piece of code.
 * */
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wvariadic-macros"

/* Print functions with the corresponding level */
#define pr_info(fmt, ...) \
	pr_level(PRINT_INFO, __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)
#define pr_debug(fmt, ...) \
	pr_level(PRINT_DEBUG, __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)
#define pr_warn(fmt, ...) \
	pr_level(PRINT_WARN, __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)
#define pr_error(fmt, ...) \
	pr_level(PRINT_ERROR, __FILE__, __func__, __LINE__, fmt, ##__VA_ARGS__)

#pragma GCC diagnostic pop

#ifndef PRINT_LEVEL
#define PRINT_LEVEL PRINT_DEBUG
#endif


/**
 * get_level_prefix - Return a text according to the given print level.
 * It is a text translation of the level number.
 *
 * @level: Print level
 * */
static inline char *get_level_prefix(level) {
	char *level_prefix;
	switch(level) {
	case PRINT_INFO:
		level_prefix = "[INFO]";
		break;
	case PRINT_DEBUG:
		level_prefix = "[DEBUG]";
		break;
	case PRINT_WARN:
		level_prefix = "[WARN]";
		break;
	case PRINT_ERROR:
		level_prefix = "[ERROR]";
		break;
	default:
		level_prefix = "";
		break;
	}
	return level_prefix;
}


/**
 * pr_level - format the program print according to the print level.
 * @level: print level being called
 * @file: file from were the function was called.
 * @func: function where the print was called.
 * @fmt: text to be formatted.
 * */
static inline void pr_level(const int level, const char *file, const char *func,
		const int line, const char *fmt, ...)
{
	char *str, *tmp_fmt;
	va_list args;

	if (level <= PRINT_LEVEL) {
		char *level_prefix = get_level_prefix(level);

		/* The new text is created considering the file, the
		 * function, the line and the inserted characters ([:::]: ),
		 * which have the size of 9 - plus 15 characters for a int max
		 * value. The final string is then printed.*/
		va_start(args, fmt);
		str = malloc(strlen(fmt) + strlen(file) + strlen(func) +
				strlen(level_prefix) + 18);

		tmp_fmt = "[%s::%s:%d]: %s %s";
		sprintf(str, tmp_fmt, file, func, line, level_prefix, fmt);

		vprintf(str, args);
		va_end(args);
	}
}

#endif
