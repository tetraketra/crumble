#ifndef C_LOG_H
#define C_LOG_H

#include "common.h"

/*
    Change the minimum log level.

    @param loglvl Log level to use as the inclusive floor (see:enum logLevel).
*/
#define LOG_LEVEL(loglvl) do { log_min_level = loglvl; } while (0)

/*
    Log something (printf-style) to a stream.

    @param stream=stderr Stream to log to.
    @param loglvl Log level to use (see:enum logLevel).
    @param fmt Text to log, with optional printf-style format specifiers.
    @param ... Optional values to be used with the format specifiers.

    @note In the future, it might be useful to put an `if` here 
    to dynamically control what's logged (by min log level).
*/
#define LOG(stream, loglvl, fmt, ...) do { if (loglvl >= log_min_level) { char timestamp[32]; strf_iso8601_utc_time(timestamp, sizeof(timestamp)); u64 len = strlen(fmt); fprintf(stream, "[%s][%s][%s:%d:%s] "fmt"%c", log_is_color_terminal ? _lglvlc[loglvl] : _lglvl[loglvl], timestamp, basename(__FILE__), __LINE__, __FUNCTION__, ##__VA_ARGS__, ((len > 0 && fmt[len - 1] == '\n') ? '\0' : '\n')); } } while (0)
typedef enum logLevel { LOG_DEBUG, LOG_INFO, LOG_WARN, LOG_ERROR, LOG_FATAL } logLevel;
static char* _lglvl[5] = { "DBG", "INF", "WRN", "ERR", "FTL" };
static char* _lglvlc[5] = { "\x1b[34mDBG\x1b[0m","\x1b[32mINF\x1b[0m","\x1b[33mWRN\x1b[0m","\x1b[31mERR\x1b[0m","\x1b[31mFTL\x1b[0m" };

extern int  log_min_level;
extern bool log_is_color_terminal;
extern void strf_iso8601_utc_time(char *buffer, size_t size);

#endif