#include "log.h"

/* Minimum log level (inclusive). */
int log_min_level = LOG_DEBUG;

/* Whether `stderr` is connected to a color terminal. Known before main but not at comptime. */
bool log_is_color_terminal = false;

static void __attribute__((constructor)) _stderr_color_support() {
    if (!isatty(fileno(stderr))) {
        return;
    }

    const char *term = getenv("TERM");
    if (!term || strcmp(term, "dumb") == 0) {
        return;
    }

    log_is_color_terminal = true;
}

/*
    Formats the current time as `YYYY-mm-ddTHH:MM:SS.SSSSSSZ`
    and prints no more than `size` bytes to `buffer`.

    @param buffer The buffer to print to.
    @param size The maximum bytes to print.

    @note Recommended buffer size of 32.
*/
void strf_iso8601_utc_time(char* buffer, usize size) {
    ASSERT_RUNTIME(buffer != NULL);

    struct timeval tv;
    gettimeofday(&tv, NULL);

    time_t sec = tv.tv_sec;
    struct tm tm;
    gmtime_r(&sec, &tm);

    int len = strftime(buffer, size, "%Y-%m-%dT%H:%M:%S", &tm);
    snprintf(buffer + len, size - len, ".%06ldZ", tv.tv_usec);
}