#include "log.h"

#include <string>
#include "glm/vec4.hpp"
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"

#include <iostream>
#include <colors.hpp>


dye::R<const char*> logLevelToString(LogLevel logLevel) {
    
    dye::R<const char *> temp; 

    switch (logLevel)
    {
        case 1:
            temp = dye::blue("INFO");
        break;
        case 2: 
            temp = dye::green("SUCCESS"); 
        break; 
        case 3:
            temp = dye::yellow("WARNING"); 
        break; 
        case 4: 
            temp = dye::red("ERROR"); 
        break;
        default:
        break; 
    }

    return temp; 
};

void log(LogLevel logLevel, std::string message) {
    std::cout << "[" << logLevelToString(logLevel) << "] " << message << std::endl; 
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