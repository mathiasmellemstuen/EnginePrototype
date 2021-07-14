#ifndef ENGINEPROTOTYPE_YAMLPARSER
#define ENGINEPROTOTYPE_YAMLPARSER

#include <string>
#include <vector>
#include <fstream>
#include <any>
#include <map>

struct YamlType {
    std::map<std::string, std::any> map;
    std::vector<std::any> vec;
};

std::vector<std::string> getTabedStrings(std::vector<std::string> lines, int tabLevel, int startLine);
std::map<std::string, std::any> loadPropFromFile(int& tabulatorNr);
std::vector<std::string> readFile(std::string fileName);

std::map<std::string, std::any> parseInlineObject(std::string line);
std::vector<std::any> parseInlineVector(std::string line);

int getTabLevel(std::string line);
char getFirstCharacter(std::string line);

template <typename T>
std::optional<T> get_v_opt(const std::any &a);

std::string buildPrint(std::any object, int tab);
std::string buildObjectPrint(std::map<std::string, std::any> object, int tab);
std::string buildVectorPrint(std::vector<std::any> vec, int tab);
std::string buildYamlTypePrint(YamlType yamlType, int tab);

void printResult();
void runTest();

#endif