#include "logging.h"
#include <glm/glm.hpp>
#include <glm/gtx/string_cast.hpp>

void logToTerminal(const char* fileName, const char* functionName, const int& lineNumber, const int& logLevel, const int& flowType, const std::string& message) {

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

    if(formattedFileName.find("\\src\\")) {
        try
        {
            formattedFileName = formattedFileName.substr(formattedFileName.find("\\src\\") + 5, formattedFileName.length() - 1);
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
		case 5:
			std::cout << rang::fg::magenta << "ASSERTION" << rang::fg::reset;
        default:
        break; 
    }
    std::cout << "] " << message;

    // Haulting program flow if hault is enabled. This will block the execution on the current thread.
    if(flowType == FlowType::HAULT) {
        std::cout << " " << rang::bg::red << rang::fg::green  << "[HAULT]" << rang::bg::reset << rang::fg::reset;
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(),'\n');
    }
    std::cout << std::endl;
};

void logToTerminal(const char* fileName, const char* functionName, const int& lineNumber, const int& logLevel, const int& flowType, const int& message) {
    logToTerminal(fileName, functionName, lineNumber, logLevel, flowType, std::to_string(message));
};
void logToTerminal(const char* fileName, const char* functionName, const int& lineNumber, const int& logLevel, const int& flowType, const float& message) {
    logToTerminal(fileName, functionName, lineNumber, logLevel, flowType, std::to_string(message));
};
void logToTerminal(const char* fileName, const char* functionName, const int& lineNumber, const int& logLevel, const int& flowType, const double& message) {
    logToTerminal(fileName, functionName, lineNumber, logLevel, flowType, std::to_string(message));
};
void logToTerminal(const char* fileName, const char* functionName, const int& lineNumber, const int& logLevel, const int& flowType, const glm::vec2& message) {
    logToTerminal(fileName, functionName, lineNumber, logLevel, flowType, glm::to_string(message));
};
void logToTerminal(const char* fileName, const char* functionName, const int& lineNumber, const int& logLevel, const int& flowType, const glm::vec3& message) {
    logToTerminal(fileName, functionName, lineNumber, logLevel, flowType, glm::to_string(message));
};
void logToTerminal(const char* fileName, const char* functionName, const int& lineNumber, const int& logLevel, const int& flowType, const glm::vec4& message) {
    logToTerminal(fileName, functionName, lineNumber, logLevel, flowType, glm::to_string(message));
}
void logToTerminal(const char* fileName, const char* functionName, const int& lineNumber, const int& logLevel, const int& flowType, const glm::mat2& message) {
    logToTerminal(fileName, functionName, lineNumber, logLevel, flowType, glm::to_string(message));
}
void logToTerminal(const char* fileName, const char* functionName, const int& lineNumber, const int& logLevel, const int& flowType, const glm::mat3& message) {
    logToTerminal(fileName, functionName, lineNumber, logLevel, flowType, glm::to_string(message));
}
void logToTerminal(const char* fileName, const char* functionName, const int& lineNumber, const int& logLevel, const int& flowType, const glm::mat4& message) {
    logToTerminal(fileName, functionName, lineNumber, logLevel, flowType, glm::to_string(message));
}

void assertToTerminal(const char* fileName, const char* functionName, const int& lineNumber, const bool& expression) {
	if(!expression) {
		logToTerminal(fileName, functionName, lineNumber, 5, 1, "Assertion expression is false. Terminating application.");
		throw std::runtime_error("Termination caused by assertion."); 
	}
}
