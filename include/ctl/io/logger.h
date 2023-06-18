#if !defined(CTL_IO_LOGGER_H)
#define CTL_IO_LOGGER_H
#include "ctl.h"
#include "ctl/terminal.h"

typedef enum CTL_LOG {
    CTL_INFO,
    CTL_TRACE,
    CTL_WARN,
    CTL_DEBUG,
    CTL_ERROR,
    CTL_FATAL,
    CTL_TEST,
} CTLLogLevel;

void CTLLogPrintf(const CTLLogLevel level, const char *format, ...);

#ifdef DEBUG
#define CTLLogTrace(format, ...) CTLLogPrintf(CTL_TRACE, format, __VA_ARGS__)
#define CTLLogDebug(format, ...) CTLLogPrintf(CTL_DEBUG, format, __VA_ARGS__)
#else
#define CTLLogTrace(format, ...) UNDEFINED
#define CTLLogDebug(format, ...) UNDEFINED
#endif

void CTLLogSetup(void);
void CTLLogTeardown(void);

#endif  // CTL_IO_LOGGER_H
