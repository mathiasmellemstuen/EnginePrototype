#ifndef ENGINEPROTOTYPE_YAMLPARSER
#define ENGINEPROTOTYPE_YAMLPARSER

#include <string>
#include <vector>
#include <fstream>
#include <any>
#include <optional>
#include <unordered_map>

#include <utility>
#include <array>

enum Types {
    NONE,
    ARRAY,
    MAP
};

struct YamlType {
    std::string tmpKey;
    int tmpIndex;

    std::unordered_map<std::string, std::any> map;
    std::vector<std::any> vec;

    YamlType operator[](const char* key);
    YamlType operator[](int index);

    operator const int();
    operator const double();
    operator const bool();
    operator const std::string();
};

class YamlParser {
    public:
        YamlType result;

        explicit YamlParser(const std::string& fileName);
        ~YamlParser();

        void print();

        YamlType operator[](std::string key);

    private:
        const char mapChar = ':';       // With a space afther (": ")
        const char listSplitChar = ','; // With a space afther (", ")
        const char arrayChar = '-';     // With a space before (" -")
        const char commentChar = '#';   // With a space before (" #")

        const char multiLineIncludeChar = '|';
        const char multiLineFoldChar = '>';

        const std::string fileStart = "---";
        const std::string fileEnd = "...";

        const std::array<std::string, 3> trueString = {"true", "yes", "on"};
        const std::array<std::string, 3> falseString = {"false", "no", "off"};

        std::vector<std::string> lines;

        YamlType parsePropsFormLines(std::vector<std::string> lines);

        std::vector<std::string> readFile(const std::string& fileName);

        std::any parseValue(const std::string& value);
        std::unordered_map<std::string, std::any> parseInlineObject(std::string object);
        std::vector<std::any> parseInlineVector(std::string vector);
        static std::string parseMultilineString(std::vector<std::string> lines, bool includeNewLine);

        bool parseBool(std::string value);
        static int parseInt(std::string value);
        static double parseDouble(const std::string& value);
        static std::string parseString(std::string value);

        static std::vector<std::string> getTabedString(std::vector<std::string> lines, int tabLevel, int startLine);

        bool isBool(std::string value);
        static bool isInt(std::string value);
        static bool isDouble(const std::string& value);

        static int getTabLevel(const std::string& line);
        static char getFirstChar(const std::string& line);
        static bool containsChar(const std::string& line, char containChar);

        static std::vector<std::string> splitString(const std::string& line, char splitChar);
        static std::string removeSpaceBeforeChar(std::string line);

        // Printing
        template <typename T>
        std::optional<T> get_v_opt(const std::any &a);

        static inline const char * boolToString(bool b);
        static inline const char * intToString(int i);
        static inline const char * doubleToString(double d);

        std::string buildPrint(const std::any& object, int tab);
        std::string buildObjectPrint(std::unordered_map<std::string, std::any> object, int tab);
        std::string buildVectorPrint(std::vector<std::any> vector, int tab);
        std::string buildYamlTypePrint(YamlType yamlType, int tab);
};

#endif