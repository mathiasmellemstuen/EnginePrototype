#include <iostream>
#include <map>
#include <any>
#include <sstream>
#include <algorithm>
#include <cmath>
#include <optional>

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

inline const char * DataParser::boolToString(bool b) {
    return b ? "false" : "true";
}

inline const char * DataParser::intToString(int i) {
    std::ostringstream strs;
    strs << i;
    std::string str = strs.str();
    return str.c_str();
}

inline const char * DataParser::doubleToString(double d) {
    std::ostringstream strs;
    strs << d;
    std::string str = strs.str();
    return str.c_str();
}


/*
    Printing
*/
template <typename T>
std::optional<T> DataParser::get_v_opt(const std::any &a) {
    if (const T *v = std::any_cast<T>(&a))
        return std::optional<T>(*v);
    else
        return std::nullopt;
}

std::string DataParser::buildPrint(const std::any& object, int tab) {
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

std::string DataParser::buildObjectPrint(std::map<std::string, std::any> object, int tab) {
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

std::string DataParser::buildVectorPrint(std::vector<std::any> vec, int tab) {
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

std::string DataParser::buildDataTypePrint(DataType DataType, int tab) {
    std::string outString = "";

    if (!DataType.map.empty()) {
        outString += buildObjectPrint(DataType.map, tab);
    }

    if (!DataType.vec.empty()) {
        outString += buildVectorPrint(DataType.vec, tab);
    }

    return outString;
}

void DataParser::print() {
    std::cout << "--- New prop ---" << std::endl;
    std::string printString = "";
    
    if (!result.map.empty()) {
        for (const auto& [key, value] : result.map) {
            printString += key + ':' + " " + buildPrint(value, 0) + "\n";
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