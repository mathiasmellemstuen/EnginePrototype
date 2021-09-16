#ifndef ENGINEPROTOTYPE_DATAPARSER
#define ENGINEPROTOTYPE_DATAPARSER

#include <string>
#include <map>
#include <vector>
#include <any>
#include <array>

struct DataType {
    std::string tmpKey;
    int tmpIndex;

    std::map<std::string, std::any> map;
    std::vector<std::any> vec;

    DataType operator[](const char* key);
    DataType operator[](int index);

    operator const int();
    operator const double();
    operator const bool();
    operator const std::string();
    operator const std::map<std::string, std::any>();
    //operator const vector<std::any>();

    void add(std::pair<std::string, std::any> pair);
    // void add(std::map<std::string, std::any> value);
    void add(const std::any& val);
};

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