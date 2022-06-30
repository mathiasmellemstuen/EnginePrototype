#ifndef ENGINEPROTOTYPE_LOGGING
#define ENGINEPROTOTYPE_LOGGING

#include <string>
#include <glm/glm.hpp>

enum LogLevel {
    INFO = 1,
    SUCCESS = 2,
    WARNING = 3,
    ERROR = 4
};

#define _WIN32_WINNT 0x0A000007
#include <rang/rang.hpp>

void logToTerminal(const char* fileName, const char* functionName, const int& lineNumber, const int& logLevel, const std::string& message);
void logToTerminal(const char* fileName, const char* functionName, const int& lineNumber, const int& logLevel, const int& message);
void logToTerminal(const char* fileName, const char* functionName, const int& lineNumber, const int& logLevel, const float& message);
void logToTerminal(const char* fileName, const char* functionName, const int& lineNumber, const int& logLevel, const double& message);
void logToTerminal(const char* fileName, const char* functionName, const int& lineNumber, const int& logLevel, const glm::vec2& message);
void logToTerminal(const char* fileName, const char* functionName, const int& lineNumber, const int& logLevel, const glm::vec3& message);
void logToTerminal(const char* fileName, const char* functionName, const int& lineNumber, const int& logLevel, const glm::vec4& message);
void logToTerminal(const char* fileName, const char* functionName, const int& lineNumber, const int& logLevel, const glm::mat2& message);
void logToTerminal(const char* fileName, const char* functionName, const int& lineNumber, const int& logLevel, const glm::mat3& message);
void logToTerminal(const char* fileName, const char* functionName, const int& lineNumber, const int& logLevel, const glm::mat4& message);
#define logger(l, m) logToTerminal(__FILE__, __func__, __LINE__, l, m)

#endif