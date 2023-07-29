#include "ctl/io/logger.h"

#include <stdarg.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#define TIME_STR_LEN (72)
#define MILLIS_STR_LEN 8

#ifdef LOG_FILE
// Determinel log file path.
#ifndef LOG_FILE_PATH
#define LOG_FILE_PATH "game.log"
#endif  // LOG_FILE_PATH

// Is file read with terminal (include colors).
#ifdef LOG_EXT_TERMINAL
#define COLORS 1
#else
#define COLORS 0
#endif
#else
// Log with color to stdout.
#define COLORS 1
#endif  // LOG_FILE

#if COLORS
#define PRETTY_INFO TERM_GRN "INFO" TERM_RSET
#define PRETTY_TRACE TERM_CYNHB "TRACE" TERM_RSET
#define PRETTY_WARN TERM_YEL "WARN" TERM_RSET
#define PRETTY_DEBUG TERM_MAG "DEBUG" TERM_RSET
#define PRETTY_ERROR TERM_RED "ERROR" TERM_RSET
#define PRETTY_FATAL TERM_REDHB "FATAL" TERM_RSET
#define PRETTY_TEST TERM_BLU "TEST" TERM_RSET
#else
#define PRETTY_INFO "INFO"
#define PRETTY_TRACE "TRACE"
#define PRETTY_WARN "WARN"
#define PRETTY_DEBG "DEBUG"
#define PRETTY_ERROR "ERROR"
#define PRETTY_FATAL "FATAL"
#define PRETTY_TEST "TEST"
#endif

static FILE *OUT = NULL;

static char *timestr(char *str) {
    // Date and time of day
    time_t now = time(NULL);
    struct tm *time_info = localtime(&now);
    strftime(str, TIME_STR_LEN, "%H:%M:%S:", time_info);

    // Append millis
    char millis[MILLIS_STR_LEN] = {0};
    struct timeval current_time;
    gettimeofday(&current_time, NULL);
    sprintf(millis, "%.3lf", (float)current_time.tv_usec / 1000);
    strncat(str, millis, MILLIS_STR_LEN);

    return str;
}

void CTLLogPrintf(const CTLLogLevel level, const char *format, ...) {
    if (OUT == NULL) {
        fprintf(stderr, "LOGGER IS NOT INITIALIZED!\n");
        return;
    }

    va_list args;
    va_start(args, format);

    switch (level) {
        case CTL_INFO:
            fprintf(OUT, "[" PRETTY_INFO " ] ");
            break;
        case CTL_TRACE:
            fprintf(OUT, "[" PRETTY_TRACE "] ");
            break;
        case CTL_WARN:
            fprintf(OUT, "[" PRETTY_WARN " ] ");
            break;
        case CTL_DEBUG:
            fprintf(OUT, "[" PRETTY_DEBUG "] ");
            break;
        case CTL_ERROR:
            fprintf(OUT, "[" PRETTY_ERROR "] ");
            break;
        case CTL_FATAL:
            fprintf(OUT, "[" PRETTY_FATAL "] ");
            break;
        case CTL_TEST:
            fprintf(OUT, "[" PRETTY_TEST " ] ");
            break;
        default:
            break;
    }

    // Time
    char str[TIME_STR_LEN] = {0};
    timestr(str);
#if COLORS
    fprintf(OUT, "[" TERM_CYN "%s" TERM_RSET "] ", str);
#else
    fprintf(OUT, "[%s] ", str);
#endif

    // message
    vfprintf(OUT, format, args);
    fprintf(OUT, "\n");

#ifdef LOG_FILE
    fflush(OUT);
#endif

    va_end(args);
}

void CTLLogTeardown(void) {
#ifdef LOG_FILE
    fclose(OUT);
#if !LOG_PERSIST
    remove(LOG_FILE_PATH);
#endif
#endif
}

void CTLLogSetup(void) {
#ifdef LOG_FILE
    FILE *stream = fopen(LOG_FILE_PATH, "a");
    if (stream == NULL) {
        perror(LOG_FILE_PATH);
        exit(EXIT_FAILURE);
    }
    OUT = stream;
#else
    OUT = stdout;
#endif
}