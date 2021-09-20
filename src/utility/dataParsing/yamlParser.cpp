#include <string>
#include <map>
#include <any>
#include <fstream>
#include <iostream>
#include <utility>
#include <vector>
#include <sstream>
#include <typeinfo>

#include <algorithm>
#include <cmath>

#include "dataParser.h"
#include "yamlParser.h"

/*
Yaml Parser
*/
YamlParser::YamlParser(const std::string& fileName) {
    lines = readFile(fileName);
    result = parsePropsFormLines(lines);
}

YamlParser::~YamlParser() = default;

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

        // Finde the ": " to determen if the line is a object or not
        std::size_t delimeterIndex = line.find(mapChar);

        if (delimeterIndex == std::string::npos) {
            // Array
            std::string val = line.erase(0, cTab + 2);
            currentYaml.add(parseValue(val));
        } else {
            // Object
            std::string left = line.substr(0, delimeterIndex).erase(0, cTab);
            std::string right = line.erase(0, delimeterIndex + 2);

            if (right != "") {
                switch (getFirstChar(right)) {
                    case '[':
                        currentYaml.add({left, parseInlineVector(right)});
                    break;
                    case '{':
                        currentYaml.add({left, parseInlineObject(right)});
                    break;
                    case '|': {
                        std::vector<std::string> nextTabLines = getTabedString(lines, nTab, i + 1);

                        i += nextTabLines.size();

                        currentYaml.add({left, parseMultilineString(nextTabLines, true)});
                    }
                    break;
                    case '>': {
                        std::vector<std::string> nextTabLines = getTabedString(lines, nTab, i + 1);

                        i += nextTabLines.size();

                        currentYaml.add({left, parseMultilineString(nextTabLines, false)});
                    }
                    break;
                    default:
                        currentYaml.add({left, parseValue(right)});
                    break;
                }
            } else {
                std::vector<std::string> nextTabLines = getTabedString(lines, nTab, i + 1);
                DataType nextObject = parsePropsFormLines(nextTabLines);

                currentYaml.add({left, nextObject});

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

    std::vector<std::string> currentString = DataParser::splitString(object, listSplitChar);

    for (std::string part : currentString) {
        std::size_t delimeterIndex = part.find(mapChar);
        std::string left = part.substr(0, delimeterIndex);
        std::string right = part.erase(0, delimeterIndex + 2);

        left = DataParser::removeSpaceBeforeChar(left);
        right = DataParser::removeSpaceBeforeChar(right);

        currentMap.insert({left, parseValue(right)});
    }

    return currentMap;
}

std::vector<std::any> YamlParser::parseInlineVector(std::string vector) {
    std::vector<std::any> currentVec;

    // Trim the string
    vector.erase(0,1);
    vector.erase(vector.size() - 1);

    auto currentString = DataParser::splitString(vector, listSplitChar);

    for (std::string part : currentString) {
        part = DataParser::removeSpaceBeforeChar(part);
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

    if (DataParser::isBool(value)) {
        return std::make_any<bool>(DataParser::parseBool(value)) ;
    } else if (DataParser::isDouble(value) && containsChar(value, '.')) {
        return std::make_any<double>(DataParser::parseDouble(value));
    } else if (DataParser::isInt(value)) {
        return std::make_any<int>(DataParser::parseInt(value));
    } else {
        return parseString(value);
    }
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






/*
    TODO: Parse value string into correct type
*/
