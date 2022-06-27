#include "logging.h"

void logToTerminal(const char* fileName, int lineNumber, int logLevel, std::string message) {

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

    std::cout << "[" << rang::fg::cyan << formattedFileName << rang::fg::reset << "][" << rang::fg::magenta << lineNumber << rang::fg::reset << "]";
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