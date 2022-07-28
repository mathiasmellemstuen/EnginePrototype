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

enum FlowType {
    CONTINUE = 1,
    HAULT = 2
};

#define _WIN32_WINNT 0x0A000007
#include <rang/rang.hpp>

void logToTerminal(const char* fileName, const char* functionName, const int& lineNumber, const int& logLevel, const int& flowType, const std::string& message);
void logToTerminal(const char* fileName, const char* functionName, const int& lineNumber, const int& logLevel, const int &flowType, const int& message);
void logToTerminal(const char* fileName, const char* functionName, const int& lineNumber, const int& logLevel, const int &flowType, const float& message);
void logToTerminal(const char* fileName, const char* functionName, const int& lineNumber, const int& logLevel, const int &flowType, const double& message);
void logToTerminal(const char* fileName, const char* functionName, const int& lineNumber, const int& logLevel, const int &flowType, const glm::vec2& message);
void logToTerminal(const char* fileName, const char* functionName, const int& lineNumber, const int& logLevel, const int& flowType, const glm::vec3& message);
void logToTerminal(const char* fileName, const char* functionName, const int& lineNumber, const int& logLevel, const int& flowType, const glm::vec4& message);
void logToTerminal(const char* fileName, const char* functionName, const int& lineNumber, const int& logLevel, const int& flowType, const glm::mat2& message);
void logToTerminal(const char* fileName, const char* functionName, const int& lineNumber, const int& logLevel, const int& flowType, const glm::mat3& message);
void logToTerminal(const char* fileName, const char* functionName, const int& lineNumber, const int& logLevel, const int& flowType, const glm::mat4& message);

#define CAT( A, B ) A ## B
#define SELECT( NAME, NUM ) CAT( NAME ## _, NUM )
#define COMPOSE( NAME, ARGS ) NAME ARGS
#define GET_COUNT( _0, _1, _2, _3, _4, _5, _6 /* ad nauseam */, COUNT, ... ) COUNT
#define EXPAND() ,,,,,, // 6 commas (or 7 empty tokens)
#define VA_SIZE( ... ) COMPOSE( GET_COUNT, (EXPAND __VA_ARGS__ (), 0, 6, 5, 4, 3, 2, 1) )
#define VA_SELECT( NAME, ... ) SELECT( NAME, VA_SIZE(__VA_ARGS__) )(__VA_ARGS__)

#define logger( ... ) VA_SELECT(logger, __VA_ARGS__)
#define logger_0() logToTerminal(__FILE__, __func__, __LINE__, 1, "Generic log message")
#define logger_1(l) logToTerminal(__FILE__, __func__, __LINE__, l, "Generic log message")
#define logger_2(l, m) logToTerminal(__FILE__, __func__, __LINE__, l, 1, m)
#define logger_3(l, f, m) logToTerminal(__FILE__, __func__, __LINE__, l, f, m)

#endif