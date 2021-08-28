#include "yamlParser.h"

#include <string>
#include <map>
#include <any>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
#include <sstream>
#include <optional>
#include <typeinfo>

#include <algorithm>
#include <cmath>

/*
Yaml Parser
*/
YamlParser::YamlParser(const std::string& fileName) {
    lines = readFile(fileName);
    result = parsePropsFormLines(lines);
}

YamlParser::~YamlParser() {

}

DataType YamlParser::operator[](std::string key) {
    DataType returnType = std::any_cast<DataType>(result.map[key]);

    return returnType;
}

std::vector<std::string> YamlParser::readFile(const std::string& fileName) {
    std::string line = "";
    std::fstream file;
    std::vector<std::string> outLines;
    std::size_t commnetIndex;

    file.open(fileName, std::ios::in);

    while (file.is_open() && std::getline(file, line)) {
        if (!line.empty()) {
            if (line.compare(0, 3, fileStart) == 0) {
                outLines.clear();   // Clear the vector, because we found the start of the YAML file
                // std::cout << "*** Clear vector ***" << std::endl;
                continue;
            } else if ((line.compare(0, 3, fileEnd) == 0)) {
                return outLines;    // Return what we have
            }

            // Skip lines that start with a #, because that line is a commnent
            if (line[0] == commentChar)
                continue;

            // Find the index of " #" because that indicates that there is a comment from that index
            commnetIndex = line.find(commentChar);

            // If no idex is found then push the entire string, else remove the commnet, then push
            if (commnetIndex == std::string::npos) {
                // std::cout << line << std::endl;
                outLines.push_back(line);
            } else {
                line.erase(commnetIndex);
                // std::cout << line << std::endl;
                outLines.push_back(line);
            }
        }
    }

    return outLines;
}

/*
Parsing
*/
DataType YamlParser::parsePropsFormLines(std::vector<std::string> lines) {
    DataType currentYaml;

    for (int i = 0; i <= lines.size() - 1; i++) {
        int cTab = getTabLevel(lines[i]);
        int nTab = i + 1 <= lines.size() - 1 ? getTabLevel(lines[i + 1]) : -1;
        std::string line = lines[i];
        bool isArray = false;

        if (line[cTab + 1] == arrayChar) {
            isArray = true;
        }

        // Finde the ": " to determen if the line is a object or not       
        std::size_t delimeterIndex = line.find(mapChar);

        if (delimeterIndex == std::string::npos) {
            std::string val = line.erase(0, cTab + 2);
            currentYaml.vec.push_back(parseValue(val));
        } else {
            std::string left = line.substr(0, delimeterIndex).erase(0, cTab);
            std::string right = line.erase(0, delimeterIndex + 2);

            if (right != "") {
                switch (getFirstChar(right)) {
                    case '[':
                        currentYaml.map.insert({left, parseInlineVector(right)});
                    break;
                    case '{':
                        currentYaml.map.insert({left, parseInlineObject(right)});
                    break;
                    case '|': {
                        std::vector<std::string> nextTabLines = getTabedString(lines, nTab, i + 1);

                        i += nextTabLines.size();

                        currentYaml.map.insert({left, parseMultilineString(nextTabLines, true)});
                    }
                    break;
                    case '>': {
                        std::vector<std::string> nextTabLines = getTabedString(lines, nTab, i + 1);

                        i += nextTabLines.size();

                        currentYaml.map.insert({left, parseMultilineString(nextTabLines, false)});
                    }
                    break;
                    default:
                        currentYaml.map.insert({left, parseValue(right)});
                    break;
                }
            } else {
                std::vector<std::string> nextTabLines = getTabedString(lines, nTab, i + 1);
                DataType nextObject = parsePropsFormLines(nextTabLines);

                currentYaml.map.insert({left, nextObject});

                i += nextTabLines.size();
            }
        }
    }

    return currentYaml;
}

std::map<std::string, std::any> YamlParser::parseInlineObject(std::string object) {
    std::map<std::string, std::any> currentMap;

    // Trim the string
    object.erase(0,1);
    object.erase(object.size() - 1);

    std::vector<std::string> currentString = splitString(object, listSplitChar);

    for (std::string part : currentString) {
        std::size_t delimeterIndex = part.find(mapChar);
        std::string left = part.substr(0, delimeterIndex);
        std::string right = part.erase(0, delimeterIndex + 2);

        left = removeSpaceBeforeChar(left);
        right = removeSpaceBeforeChar(right);

        currentMap.insert({left, parseValue(right)});
    }

    return currentMap;
}

std::vector<std::any> YamlParser::parseInlineVector(std::string vector) {
    std::vector<std::any> currentVec;

    // Trim the string
    vector.erase(0,1);
    vector.erase(vector.size() - 1);

    auto currentString = splitString(vector, listSplitChar);

    for (std::string part : currentString) {
        part = removeSpaceBeforeChar(part);
        currentVec.push_back(parseValue(part));
    }

    return currentVec;
}

std::string YamlParser::parseMultilineString(std::vector<std::string> lines, bool includeNewLine) {
    std::string outString = "";
    int spaceLevel = getTabLevel(lines[0]);

    for (int i = 0; i < lines.size(); i++) {
        std::string line = lines[i];

        line.erase(0, (i == 0 ? spaceLevel : spaceLevel - 1));

        outString += line;
        outString += (includeNewLine && i != lines.size() - 1) ? "\n" : "";
    }

    return outString;
}

std::any YamlParser::parseValue(const std::string& value) {
    std::any returnValue;

    if (isBool(value)) {
        return std::make_any<bool>(parseBool(value)) ;
    } else if (isDouble(value) && containsChar(value, '.')) {
        return std::make_any<double>(parseDouble(value));
    } else if (isInt(value)) {
        return std::make_any<int>(parseInt(value));
    } else {
        return parseString(value);
    }
}

bool YamlParser::parseBool(std::string value) {
    // Turn string to lower
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c){return std::tolower(c);});

    // Find if value is a boolean
    if (std::find(std::begin(trueString), std::end(trueString), value) != std::end(trueString)) {
        // value is part of the trueString list
        return true;
    } else if (std::find(std::begin(falseString), std::end(falseString), value) != std::end(falseString)) {
        // value is part of the falseString list 
        return false;
    }

    return true;
}

int YamlParser::parseInt(std::string value) {
    if (value[0] == '0' && value[1] == 'x') {
        return std::stoi(value, 0, 16);
    } else if (value[0] == '0') {
        return std::stoi(value, 0, 6);
    } else {
        return std::stoi(value);
    }
}

double YamlParser::parseDouble(const std::string& value) {
    char* end = nullptr;
    double val = strtod(value.c_str(), &end);

    return val;
}

std::string YamlParser::parseString(std::string value) {
    if (value[0] == '"' || value[0] == '\'') {
        return value.erase(0, 1).erase(value.size() - 1, 1);
    }

    return value;
}

/*
Utility
*/
char YamlParser::getFirstChar(const std::string& line) {
    for (char c : line)
        if (c != ' ')
            return c;
    return ' ';   
}

bool YamlParser::containsChar(const std::string& line, char containChar) {
    for (char c : line)
        if (c == containChar)
            return true;

    return false;
}

bool YamlParser::isBool(std::string value) {
    // Turn string to lower
    std::transform(value.begin(), value.end(), value.begin(), [](unsigned char c){return std::tolower(c);});

    // Find if value is a boolean
    if (std::find(std::begin(trueString), std::end(trueString), value) != std::end(trueString)) {
        // value is part of the trueString list
        return true;
    } else if (std::find(std::begin(falseString), std::end(falseString), value) != std::end(falseString)) {
        // value is part of the falseString list 
        return true;
    }

    return false;
}

bool YamlParser::isInt(std::string value) {
    char* p;

    if (value[0] == '0' && value[1] == 'x') {
        return true;
    } else if (value[0] == '0') {
        return true;
    } else {
        long converted = strtol(value.c_str(), &p, 10);
    }

    return !(*p);
}

bool YamlParser::isDouble(const std::string& value) {
    char* end = nullptr;
    double val = strtod(value.c_str(), &end);
    return end != value.c_str() && *end == '\0' && val != HUGE_VAL;
}

std::vector<std::string> YamlParser::splitString(const std::string& line, char splitChar) {
    std::stringstream stream(line);
    std::string segment;
    std::vector<std::string> splitString;

    while (std::getline(stream, segment, splitChar)) {
        splitString.push_back(segment);
    }

    return splitString;   
}

std::string YamlParser::removeSpaceBeforeChar(std::string line) {
    return line.erase(0, getTabLevel(line));
}

int YamlParser::getTabLevel(const std::string& line) {
    int tabLevel = 0;

    for (char c : line) {
        if (c == ' ') {
            tabLevel++;
        } else {
            return tabLevel;
        }
    }

    return tabLevel;
}

std::vector<std::string> YamlParser::getTabedString(std::vector<std::string> lines, int tabLevel, int startLine) {
    std::vector<std::string> returnString;

    for (int i = startLine; i < lines.size(); i++) {
        int cTab = getTabLevel(lines[i]);

        if (cTab >= tabLevel) {
            returnString.push_back(lines[i]);
        } else {
            return returnString;
        }
    }

    return returnString;
}

/*
Print
*/
template <typename T>
std::optional<T> YamlParser::get_v_opt(const std::any &a) {
    if (const T *v = std::any_cast<T>(&a))
        return std::optional<T>(*v);
    else
        return std::nullopt;
}

inline const char * YamlParser::boolToString(bool b) {
    return b ? "false" : "true";
}

inline const char * YamlParser::intToString(int i) {
    std::ostringstream strs;
    strs << i;
    std::string str = strs.str();
    return str.c_str();
}

inline const char * YamlParser::doubleToString(double d) {
    std::ostringstream strs;
    strs << d;
    std::string str = strs.str();
    return str.c_str();
}

std::string YamlParser::buildPrint(const std::any& object, int tab) {
    std::string printString = "";

    // Test if the object is a string
    //std::optional opt_string = get_v_opt<std::string>(object);
    std::optional opt_string = get_v_opt<std::string>(object);
    if (opt_string.has_value()) {
        return "\"" + opt_string.value() + "\"";
    }

    // Test if the object is a bool
    std::optional opt_bool = get_v_opt<bool>(object);
    if (opt_bool.has_value()) {
        return boolToString(opt_bool.value() == 0);
    }

    std::optional opt_int = get_v_opt<int>(object);
    if (opt_int.has_value()) {
        return intToString(opt_int.value());
    }

    std::optional opt_double = get_v_opt<double>(object);
    if (opt_double.has_value()) {
        return doubleToString(opt_double.value());
    }
 
    // Test if the object is another object
    std::optional opt_object = get_v_opt<std::map<std::string, std::any>>(object);
    if (opt_object.has_value()) {
        return buildObjectPrint(opt_object.value(), tab + 1);
    }

    std::optional opt_vector = get_v_opt<std::vector<std::any>>(object);
    if (opt_vector.has_value()) {
        return buildVectorPrint(opt_vector.value(), tab + 1);
    }

    std::optional opt_DataType = get_v_opt<DataType>(object);
    if (opt_DataType.has_value()) {
        return buildDataTypePrint(opt_DataType.value(), tab + 1);
    }

    return printString;
}

std::string YamlParser::buildObjectPrint(std::map<std::string, std::any> object, int tab) {
    std::string outString = "";

    for (const auto& [key, value] : object) {
        outString += "\n";

        for (int i = 0; i < tab; i++) {
            outString += "  ";
        }

        outString += key + ": " + buildPrint(value, tab);
    }

    return outString;
}

std::string YamlParser::buildVectorPrint(std::vector<std::any> vec, int tab) {
    std::string outString = "";

    for (const auto& value : vec) {
        outString += "\n";

        for (int i = 0; i < tab; i++) {
            outString += "  ";
        }

        outString += "- " + buildPrint(value, tab);
    }

    return outString;
}

std::string YamlParser::buildDataTypePrint(DataType DataType, int tab) {
    std::string outString = "";

    if (!DataType.map.empty()) {
        outString += buildObjectPrint(DataType.map, tab);
    }

    if (!DataType.vec.empty()) {
        outString += buildVectorPrint(DataType.vec, tab);
    }

    return outString;
}

void YamlParser::print() {
    std::cout << "--- New prop ---" << std::endl;
    std::string printString = "";
    
    if (!result.map.empty()) {
        for (const auto& [key, value] : result.map) {
            printString += key + mapChar + " " + buildPrint(value, 0) + "\n";
        }
    }

    if (!result.vec.empty()) {
        for (const auto& value : result.vec) {
            printString += "[ " + buildPrint(value, 0) + "\n";
        }
    }

    std::cout << printString << std::endl;
    std::cout << "--- End of new prop ---" << std::endl;
}

/*
    TODO: Parse value string into correct type
*/
