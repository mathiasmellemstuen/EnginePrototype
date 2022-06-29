#ifndef ENGINEPROTOTYPE_LOGGING
#define ENGINEPROTOTYPE_LOGGING

#include <string>

enum LogLevel {
    INFO = 1,
    SUCCESS = 2,
    WARNING = 3,
    ERROR = 4
};

#define _WIN32_WINNT 0x0A000007
#include <rang/rang.hpp>

void logToTerminal(const char* fileName, const char* functionName, int lineNumber, int logLevel, std::string message);
#define logger(l, m) logToTerminal(__FILE__, __func__, __LINE__, l, m)

#endif