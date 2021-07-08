#ifndef ENGINEPROTOTYPE_YAMLPARSER
#define ENGINEPROTOTYPE_YAMLPARSER

#include <string>
#include <fstream>
#include <any>
#include <map>

std::map<std::string, std::any> loadPropFromFile(int tabulatorNr, int& lineNr, std::fstream& file);

template <typename T>
std::optional<T> get_v_opt(const std::any &a);

void runTest();

std::string buildPrint(std::any object);
void printResult();

#endif