#ifndef _H_LOG_H_
#define _H_LOG_H_

enum LOG_LEVEL
{
    Debug = 1,
    Warning,
    Error
};

void logDebug(const char* file, int line, const char* function, LOG_LEVEL level, const char* format, ...);

void logWarning(const char* file, int line, const char* function, LOG_LEVEL level, const char* format, ...);

void logError(const char* file, int line, const char* function, LOG_LEVEL level, const char* format, ...);

#if WIN32

#define __FILENAME__ (strrchr(__FILE__, '\\') ? (strrchr(__FILE__, '\\') + 1):__FILE__)

#define LOG_DEBUG(format, ...)    (logDebug(__FILENAME__,__LINE__,__FUNCTION__,LOG_LEVEL::Debug,format,__VA_ARGS__))
#define LOG_WARNING(format, ...)  (logWarning(__FILENAME__,__LINE__,__FUNCTION__,LOG_LEVEL::Warning,format,__VA_ARGS__))
#define LOG_ERROR(format, ...)    (logError(__FILENAME__,__LINE__,__FUNCTION__,LOG_LEVEL::Error,format,__VA_ARGS__))


#else

#define __FILENAME__ (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1):__FILE__)

#define LOG_DEBUG(format, ...)    (logDebug(__FILENAME__,__LINE__,__PRETTY_FUNCTION__,LOG_LEVEL::Debug,format,##__VA_ARGS__))
#define LOG_WARNING(format, ...)  (logWarning(__FILENAME__,__LINE__,__PRETTY_FUNCTION__,LOG_LEVEL::Warning,format,##__VA_ARGS__))
#define LOG_ERROR(format, ...)    (logError(__FILENAME__,__LINE__,__PRETTY_FUNCTION__,LOG_LEVEL::Error,format,##__VA_ARGS__))

#endif

#endif  //_H_LOG_H_