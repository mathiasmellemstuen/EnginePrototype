#include <iostream>
#include <map>
#include <any>

#include "dataType.h"

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