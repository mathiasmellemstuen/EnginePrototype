#ifndef ENGINEPROTOTYPE_YAMLPARSER
#define ENGINEPROTOTYPE_YAMLPARSER

#include <string>
#include <vector>
#include <fstream>
#include <any>
#include <optional>
#include <map>

struct YamlType {
    std::map<std::string, std::any> map;
    std::vector<std::any> vec;
};

std::vector<std::string> getTabedStrings(std::vector<std::string> lines, int tabLevel, int startLine);
YamlType loadPropFromLines(std::vector<std::string> lines);
std::vector<std::string> readFile(std::string fileName);

std::map<std::string, std::any> parseInlineObject(std::string line);
std::vector<std::any> parseInlineVector(std::string line);

int getTabLevel(std::string line);
char getFirstCharacter(std::string line);
std::vector<std::string> splitString(std::string line, char splitChar);

struct YamlType {
    std::any data;
    Types type;

std::string buildPrint(std::any object, int tab);
std::string buildObjectPrint(std::map<std::string, std::any> object, int tab);
std::string buildVectorPrint(std::vector<std::any> vec, int tab);
std::string buildYamlTypePrint(YamlType yamlType, int tab);

    // For type MAP
    void add(std::pair<std::string, std::any> val);
};

class YamlParser {
    public:
        YamlType result;

        YamlParser(const std::string fileName);
        ~YamlParser();

        void print();

        //void runTest();

    private:
        const char mapChar = ':';       // With a space afther (": ")
        const char listSplitChar = ','; // With a space afther (", ")
        const char arrayChar = '-';     // With a space before (" -")
        const char commentChar = '#';   // With a space before (" #")

        const std::string fileStart = "---";
        const std::string fileEnd = "...";

        const std::array<std::string, 3> trueString = {"true", "yes", "on"};
        const std::array<std::string, 3> falseString = {"false", "no", "off"};

        std::vector<std::string> lines;

        YamlType parsePropsFormLines(std::vector<std::string> lines);

        std::vector<std::string> readFile(const std::string fileName);

        std::any parseValue(std::string value);
        bool parseBool(std::string value);
        std::map<std::string, std::any> parseInlineObject(std::string object);
        std::vector<std::any> parseInlineVector(std::string vector);

        std::vector<std::string> getTabedString(std::vector<std::string> lines, int tabLevel, int startLine);

        bool isBool(std::string value);

        int getTabLevel(std::string line);
        char getFirstChar(std::string line);
        Types getType(std::string line);

        std::vector<std::string> splitString(std::string line, char splitChar);

        // Printing
        template <typename T>
        std::optional<T> get_v_opt(const std::any &a);

        std::string buildPrint(std::any object, int tab);
        std::string buildObjectPrint(std::map<std::string, std::any> object, int tab);
        std::string buildVectorPrint(std::vector<std::any> vector, int tab);
        std::string buildYamlTypePrint(YamlType yamlType, int tab);
};








/*
struct YamlField {
    std::any* data;

    YamlField operator[](const std::string& str);
    explicit operator int(); 
};

class YamlParser {
    public:
        YamlField operator[](const std::string& str);
        std::map<std::string, std::any> data;
        YamlParser(const std::string& filePath);
        ~YamlParser();
        std::string toString();
    private:
        const char delimeter = ':';
        std::string toStringRecursive(std::map<std::string, std::any>& obj);
        std::vector<std::string> getTabedStrings(std::vector<std::string>& lines, int tabLevel, int startLine);
        std::vector<std::string> readFile(const std::string& fileName);
        std::map<std::string, std::any> loadPropFromLines(std::vector<std::string>& lines);
        int getTabLevel(std::string& line);
        template <typename T> std::optional<T> get_v_opt(const std::any &a);
};
*/
#endif