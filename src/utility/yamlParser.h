#ifndef ENGINEPROTOTYPE_YAMLPARSER
#define ENGINEPROTOTYPE_YAMLPARSER

#include <string>
#include <vector>
#include <fstream>
#include <any>
#include <optional>
#include <map>

struct YamlField {
    std::any& data;

    YamlField operator[](const std::string& str);
    //std::string operator=(const YamlField& field);
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
#endif