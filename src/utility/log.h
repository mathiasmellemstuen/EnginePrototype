#ifndef ENGINEPROTOTYPE_LOG
#define ENGINEPROTOTYPE_LOG

#include <string>
#include "glm/vec3.hpp"
#include "glm/vec2.hpp"
#include "glm/vec4.hpp"

enum LogLevel {
    INFO = 1,
    SUCCESS = 2,
    WARNING = 3,
    ERROR = 4
};

void log(LogLevel logLevel, std::string message);

void log(LogLevel logLevel, glm::vec2 vec2);
void log(LogLevel logLevel, glm::vec3 vec3); 
void log(LogLevel logLevel, glm::vec4 vec4); 

void log(LogLevel logLevel, int value);
void log(LogLevel logLevel, float value); 

void log(int value); 
void log(float value); 

void log(std::string message); 
void log(glm::vec2 vec2); 
void log(glm::vec3 vec3); 
void log(glm::vec4 vec4); 

#endif