#include "dataParser.h"

#include <iostream>
#include <map>
#include <any>
#include <sstream>
#include <algorithm>
#include <cmath>

/*
    DataType
*/
DataType DataType::operator[](const char* key) {
    tmpKey = key;

    try {
        DataType returnValue = std::any_cast<DataType>(map[key]);
        return returnValue;
    } catch (std::bad_any_cast e) {
        return *this;
    }
}

DataType DataType::operator[](int index) {
    tmpIndex = index;

    try {
        DataType returnValue = std::any_cast<DataType>(vec[index]);
        return returnValue;
    } catch (std::bad_any_cast e) {
        return *this;
    }
}

DataType::operator const int() {
    if (!map.empty()) {
        return std::any_cast<int>(map[tmpKey]);
    } else if (!vec.empty()) {
        return std::any_cast<int>(vec[tmpIndex]);
    }
    return 0;
}

DataType::operator const double() {
    if (!map.empty()) {
        return std::any_cast<double>(map[tmpKey]);
    } else if (!vec.empty()) {
        return std::any_cast<double>(vec[tmpIndex]);
    }
    return 0.0;
}

DataType::operator const bool() {
    if (!map.empty()) {
        return std::any_cast<bool>(map[tmpKey]);
    } else if (!vec.empty()) {
        return std::any_cast<bool>(vec[tmpIndex]);
    }
    return false;
}

DataType::operator const std::string() {
    if (!map.empty()) {
        return std::any_cast<std::string>(map[tmpKey]);
    } else if (!vec.empty()) {
        return std::any_cast<std::string>(vec[tmpIndex]);
    }
    return "";
}

DataType::operator const std::map<std::string, std::any>() {
    std::map<std::string, std::any> retMap = {};

    try {
        std::map<std::string, std::any> retMap = std::any_cast<std::map<std::string, std::any>>(map[tmpKey]); 
    } catch (const std::bad_any_cast& e) {
        // Debug::log(ERROR, e.what());
    }

    return retMap;
}

void DataType::add(std::pair<std::string, std::any> pair) {
    // std::cout << pair.first << std::endl;
    map.insert(pair);
}

void DataType::add(const std::any& val) {
    vec.push_back(val);
}


/*
    DataParser
*/
const std::array<std::string, 3> DataParser::trueString = {"true", "yes", "on"};
const std::array<std::string, 3> DataParser::falseString = {"false", "no", "off"};

DataType DataParser::operator[](const std::string& key) {
    return std::any_cast<DataType>(result.map[key]);
}

DataType DataParser::operator[](const int& idx) {
    return std::any_cast<DataType>(result.vec[idx]);
}

std::vector<std::string> DataParser::splitString(const std::string &data, char splitChar) {
    std::stringstream stream(data);
    std::string segment;
    std::vector<std::string> splitString;

    while (std::getline(stream, segment, splitChar)) {
        splitString.push_back(segment);
    }

    return splitString;
}

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

