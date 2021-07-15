#include "log.h"

#include <string>
#include <glm/vec4.hpp>
#include <glm/vec3.hpp>
#include <glm/vec2.hpp>
#include <iostream>
#include <rang.hpp>

void log(LogLevel logLevel, std::string message) {
    
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

void log(LogLevel logLevel, glm::vec2 vec2) {
    log(logLevel, "X: " + std::to_string(vec2.x) + " Y: " + std::to_string(vec2.y));
};

void log(LogLevel logLevel, glm::vec3 vec3) {
    log(logLevel, "X: " + std::to_string(vec3.x) + " Y: " + std::to_string(vec3.y) + " Z: " + std::to_string(vec3.z));
};

void log(LogLevel logLevel, glm::vec4 vec4) {
   log(logLevel, "X: " + std::to_string(vec4.x) + " Y: " + std::to_string(vec4.y) + " Z: " + std::to_string(vec4.z) + " W: " + std::to_string(vec4.w));
};

void log(std::string message) {
    log(INFO, message); 
};

void log(glm::vec2 vec2) {
    log(INFO, vec2); 
};

void log(glm::vec3 vec3)  {
    log(INFO, vec3); 
};

void log(glm::vec4 vec4) {
    log(INFO, vec4); 
};

void log(LogLevel logLevel, int value) {
    log(logLevel, std::to_string(value));
};

void log(LogLevel logLevel, float value) {
    log(logLevel, std::to_string(value));
};

void log(int value) {
    log(INFO, value); 
};

void log(float value) {
    log(INFO, value); 
};