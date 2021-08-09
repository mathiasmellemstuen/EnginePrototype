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

class Debug {
    public:
        static void log(LogLevel logLevel, std::string message);

        static void log(LogLevel logLevel, glm::vec2 vec2);
        static void log(LogLevel logLevel, glm::vec3 vec3); 
        static void log(LogLevel logLevel, glm::vec4 vec4); 

        static void log(LogLevel logLevel, int value);
        static void log(LogLevel logLevel, float value); 

        static void log(int value); 
        static void log(float value);

        static void log(std::string message); 
        static void log(glm::vec2 vec2); 
        static void log(glm::vec3 vec3); 
        static void log(glm::vec4 vec4); 
};

#endif