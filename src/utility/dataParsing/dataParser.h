#ifndef ENGINEPROTOTYPE_DATAPARSER
#define ENGINEPROTOTYPE_DATAPARSER

#include <string>
#include <map>
#include <vector>
#include <any>
#include <array>

#include "dataType.h"

class DataParser {
    public:
        DataType result;

        static const std::array<std::string, 3> trueString;
        static const std::array<std::string, 3> falseString;

        DataType operator[](const std::string& key);
        DataType operator[](const int& idx);

        // Util
        static std::vector<std::string> splitString(const std::string& data, char splitChar);
        static std::string removeSpaceBeforeChar(std::string line);
        static int getTabLevel(const std::string& line);
        
        // Parsing
        static bool isBool(std::string value);
        static bool isInt(const std::string& value);
        static bool isDouble(const std::string& value);

        static bool parseBool(std::string value);
        static int parseInt(std::string value);
        static double parseDouble(std::string value);
        static std::string parseString(std::string value);

};

#endif