#ifndef ENGINEPROTOTYPE_YAMLPARSER
#define ENGINEPROTOTYPE_YAMLPARSER

#include <string>
#include <vector>
#include <fstream>
#include <any>
#include <map>

std::vector<std::string> getTabedStrings(std::vector<std::string> lines, int tabLevel, int startLine);
std::map<std::string, std::any> loadPropFromFile(int& tabulatorNr);
std::vector<std::string> readFile(std::string fileName);

int getTabLevel(std::string line);

template <typename T>
std::optional<T> get_v_opt(const std::any &a);

std::string buildObjectPrint(std::map<std::string, std::any> object, int tab);
std::string buildPrint(std::any object, int tab);

void printResult();
void runTest();

#endif