#include <iostream>
#include <map>
#include <any>
#include <sstream>
#include <algorithm>
#include <cmath>

#include "dataParser.h"

const std::array<std::string, 3> DataParser::trueString = {"true", "yes", "on"};
const std::array<std::string, 3> DataParser::falseString = {"false", "no", "off"};

DataType DataParser::operator[](const std::string& key) {
    return std::any_cast<DataType>(result.map[key]);
}

DataType DataParser::operator[](const int& idx) {
    return std::any_cast<DataType>(result.vec[idx]);
}

/*
    Util
*/
std::vector<std::string> DataParser::splitString(const std::string &data, char splitChar) {
    std::stringstream stream(data);
    std::string segment;
    std::vector<std::string> splitString;

    while (std::getline(stream, segment, splitChar)) {
        splitString.push_back(segment);
    }

    return splitString;
}

std::string DataParser::removeSpaceBeforeChar(std::string line) {
    return line.erase(0, DataParser::getTabLevel(line));
}

int DataParser::getTabLevel(const std::string& line) {
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

/*
    Parsing
*/
bool DataParser::isBool(std::string value) {
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

bool DataParser::isInt(const std::string &value) {
    char* p;

    if (value.empty())
        return false;

    if (value[0] == '0' && value[1] == 'x') {
        return true;
    } else if (value[0] == '0') {
        return true;
    } else {
        long converted = strtol(value.c_str(), &p, 10);
    }

    return !(*p);
}

bool DataParser::isDouble(const std::string &value) {
    char* end = nullptr;
    double val = strtod(value.c_str(), &end);
    return end != value.c_str() && *end == '\0' && val != HUGE_VAL;
}

bool DataParser::parseBool(std::string value) {
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

int DataParser::parseInt(std::string value) {
    if (value[0] == '0' && value[1] == 'x') {
        return std::stoi(value, 0, 16);
    } else if (value[0] == '0') {
        return std::stoi(value, 0, 6);
    } else {
        return std::stoi(value);
    }
}

double DataParser::parseDouble(std::string value) {
    char* end = nullptr;
    double val = strtod(value.c_str(), &end);

    return val;
}

std::string DataParser::parseString(std::string value) {
    if (value[0] == '"' || value[0] == '\'')
        value = value.erase(0, 1);

    if (value[value.length()-1] == '"' || value[value.length()-1] == '\'')
        value = value.erase(value.length()-1, 1);

    return value;
}
