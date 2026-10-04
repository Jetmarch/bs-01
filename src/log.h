#ifndef LOG_H
#define LOG_H

#include "raylib.h"

#ifdef DEBUG
#define DEBUG_LOG(...) TraceLog(LOG_DEBUG, __VA_ARGS__)
#else
#define DEBUG_LOG(...) ((void)0)
#endif

#define INFO_LOG(...) TraceLog(LOG_INFO, __VA_ARGS__)
#define WARN_LOG(...) TraceLog(LOG_WARNING, __VA_ARGS__)
#define ERROR_LOG(...) TraceLog(LOG_ERROR, __VA_ARGS__)

#endif