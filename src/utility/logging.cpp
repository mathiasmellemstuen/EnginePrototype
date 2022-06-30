#include "logging.h"
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>
void logToTerminal(const char* fileName, const char* functionName, const int& lineNumber, const int& logLevel, const std::string& message) {

    std::string formattedFileName = fileName;
    
    if(formattedFileName.find("/src/")) {
        try
        {
            formattedFileName = formattedFileName.substr(formattedFileName.find("/src/") + 5, formattedFileName.length() - 1);
        }
        catch(const std::exception& e) {
            formattedFileName = fileName;
        }
    }

    std::cout << "[" << rang::fg::cyan << formattedFileName << rang::fg::reset << "][" << rang::fg::green << functionName << rang::fg::reset << "][" <<  rang::fg::magenta << lineNumber << rang::fg::reset << "]";
    std::cout << "["; 
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
    std::cout << "] " << message << std::endl;
};


void logToTerminal(const char* fileName, const char* functionName, const int& lineNumber, const int& logLevel, const int& message) {
    logToTerminal(fileName, functionName, lineNumber, logLevel, std::to_string(message)); 
};
void logToTerminal(const char* fileName, const char* functionName, const int& lineNumber, const int& logLevel, const float& message) {
    logToTerminal(fileName, functionName, lineNumber, logLevel, std::to_string(message)); 
};
void logToTerminal(const char* fileName, const char* functionName, const int& lineNumber, const int& logLevel, const double& message) {
    logToTerminal(fileName, functionName, lineNumber, logLevel, std::to_string(message)); 
};
void logToTerminal(const char* fileName, const char* functionName, const int& lineNumber, const int& logLevel, const glm::vec2& message) {
    logToTerminal(fileName, functionName, lineNumber, logLevel, glm::to_string(message)); 
};
void logToTerminal(const char* fileName, const char* functionName, const int& lineNumber, const int& logLevel, const glm::vec3& message) {
    logToTerminal(fileName, functionName, lineNumber, logLevel, glm::to_string(message)); 
};
void logToTerminal(const char* fileName, const char* functionName, const int& lineNumber, const int& logLevel, const glm::vec4& message) {
    logToTerminal(fileName, functionName, lineNumber, logLevel, glm::to_string(message)); 
}
void logToTerminal(const char* fileName, const char* functionName, const int& lineNumber, const int& logLevel, const glm::mat2& message) {
    logToTerminal(fileName, functionName, lineNumber, logLevel, glm::to_string(message)); 
}
void logToTerminal(const char* fileName, const char* functionName, const int& lineNumber, const int& logLevel, const glm::mat3& message) {
    logToTerminal(fileName, functionName, lineNumber, logLevel, glm::to_string(message)); 
}
void logToTerminal(const char* fileName, const char* functionName, const int& lineNumber, const int& logLevel, const glm::mat4& message) {
    logToTerminal(fileName, functionName, lineNumber, logLevel, glm::to_string(message)); 
}