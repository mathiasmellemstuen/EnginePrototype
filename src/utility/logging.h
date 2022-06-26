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

void logToTerminal(const char* fileName, int lineNumber, int logLevel, std::string message) {
        std::cout  << "["; 
        switch (logLevel) {
            case 1:
                std::cout << rang::fg::blue << "INFO" << rang::fg::reset;
            break;
            case 2: 
                std::cout << rang::fg::green << "SUCCESS" << rang::fg::reset; 
            break; 
            case 3:
                std::cout << rang::fg::yellow << "WARNING" << rang::fg::reset; 
            break; 
            case 4: 
                std::cout << rang::fg::red << "ERROR" << rang::fg::reset; 
            break;
            default:
            break; 
        }
        std::cout << "] " << "[" << rang::fg::cyan << fileName << rang::fg::reset << "][" << rang::fg::magenta << lineNumber << rang::fg::reset << "]" << message << std::endl;
};


#define logger(l, m) logToTerminal(__FILE__, __LINE__, l, m)

#endif