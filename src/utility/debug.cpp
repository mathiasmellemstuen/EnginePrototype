#include "debug.h"

#include <string>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <iostream>

#define _WIN32_WINNT 0x0A000007
#include <rang.hpp>

void Debug::log(LogLevel logLevel, std::string message) {
    
    //We will not pring anything if NOTDEBUG is defined. This should in theory also make the compiler remove all the log functions under compiling if NOTDEBUG is defined.
    #ifndef NOTDEBUG

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
    #endif
};

void Debug::log(LogLevel logLevel, glm::vec2 vec2) {
    Debug::log(logLevel, "X: " + std::to_string(vec2.x) + " Y: " + std::to_string(vec2.y));
};

void Debug::log(LogLevel logLevel, glm::vec3 vec3) {
    Debug::log(logLevel, "X: " + std::to_string(vec3.x) + " Y: " + std::to_string(vec3.y) + " Z: " + std::to_string(vec3.z));
};

void Debug::log(LogLevel logLevel, glm::vec4 vec4) {
   Debug::log(logLevel, "X: " + std::to_string(vec4.x) + " Y: " + std::to_string(vec4.y) + " Z: " + std::to_string(vec4.z) + " W: " + std::to_string(vec4.w));
};

void Debug::log(std::string message) {
    Debug::log(INFO, message); 
};

void Debug::log(glm::vec2 vec2) {
    Debug::log(INFO, vec2); 
};

void Debug::log(glm::vec3 vec3)  {
    Debug::log(INFO, vec3); 
};

void Debug::log(glm::vec4 vec4) {
    Debug::log(INFO, vec4); 
};

void Debug::log(LogLevel logLevel, int value) {
    Debug::log(logLevel, std::to_string(value));
};

void Debug::log(LogLevel logLevel, float value) {
    Debug::log(logLevel, std::to_string(value));
};

void Debug::log(LogLevel logLevel, double value) {
    Debug::log(logLevel, std::to_string(value));
}

void Debug::log(int value) {
    Debug::log(INFO, value); 
};

void Debug::log(float value) {
    Debug::log(INFO, value); 
};

void Debug::log(double value) {
    Debug::log(INFO, value);
}