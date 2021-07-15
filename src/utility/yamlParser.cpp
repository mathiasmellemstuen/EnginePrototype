#include "yamlParser.h"
#include "log.h"

#include <string>
#include <map>
#include <any>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <optional>
#include <typeinfo>

YamlField YamlField::operator[](const std::string& str) {
    std::optional optionalMap = std::any_cast<std::map<std::string, std::any>>(*data)[str];
    
    if(optionalMap.has_value()) {
        return { &std::any_cast<std::map<std::string, std::any>>(*data)[str] };
    }
     
    return *this;
};
YamlField::operator int() {
    return std::any_cast<int>(*data); 
}
//std::string YamlField::operator=(const YamlField& field) {
//    return std::any_cast<std::string>(field.data);
//};
YamlField YamlParser::operator[](const std::string& str) {
    return {&data[str]};  
};

YamlParser::YamlParser(const std::string& filePath) {
    auto lines = readFile(filePath);
    data = loadPropFromLines(lines);
};

YamlParser::~YamlParser() {};

std::vector<std::string> YamlParser::getTabedStrings(std::vector<std::string>& lines, int tabLevel, int startLine) {
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

std::map<std::string, std::any> YamlParser::loadPropFromLines(std::vector<std::string>& lines) {
    
    std::map<std::string, std::any> currentMap;

    for (int i = 0; i <= lines.size() - 1; i++) {
        
        std::string line = lines[i];
        
        int cTab = getTabLevel(line);
        int nTab = i + 1 <= lines.size() - 1 ? getTabLevel(lines[i + 1]) : -1;
        
        // Finde the index of the delimeter
        int delimeterIndex = line.find(delimeter);

        // Split into key and value
        std::string left = line.substr(0, delimeterIndex).erase(0, cTab);  // split on the delimeterIndex, then remove spaces inform of the key
        std::string right = line.erase(0, delimeterIndex + 2);

        // Check if right is a value or a object (right is "" if it is a object)
        if (right != "" || nTab == -1) {
            currentMap.insert({left, right});
        } else {

            auto nextTabLines = getTabedStrings(lines, nTab, i + 1);
            auto nextObject = loadPropFromLines(nextTabLines);

            currentMap.insert({left, nextObject});

            i += nextTabLines.size();
        }
    }
    return currentMap;
}

int YamlParser::getTabLevel(std::string& line) {
    int tabLevel = 0;

    for (char c : line) {
        if (c == 9) { //9 is the ascii code for horizontal tab.
            tabLevel++;
        } else {
            return tabLevel;
        }
    }

    return tabLevel;
}

// Place each line of the file into a vector
std::vector<std::string> YamlParser::readFile(const std::string& fileName) {
    log(INFO, "Starting reading file!"); 
    std::string line = "";
    std::fstream file;
    std::vector<std::string> outLines;

    file.open(fileName, std::ios::in);

    while (file.is_open() && std::getline(file, line)) {
        outLines.push_back(line);
    }
    
    log(SUCCESS, "File reading done."); 
    return outLines;
}


// Convert a std::any to a optional, but with a type
template <typename T> std::optional<T> YamlParser::get_v_opt(const std::any &a) {
    if (const T *v = std::any_cast<T>(&a))
        return std::optional<T>(*v);
    else
        return std::nullopt;
}

std::string YamlParser::toString() {
    return toStringRecursive(data); 
};

// Convert the object to a string, so it can be printed
std::string YamlParser::toStringRecursive(std::map<std::string, std::any>& obj) {
    std::string valueString = "";

    for(const auto& [key, value] : obj) {

        // Test if the object is a string
        std::optional opt_string = get_v_opt<std::string>(value);
        if (opt_string.has_value()) {
            valueString += "\n" + key + " : " + opt_string.value();
        }

        // Test if the object is another object
        std::optional opt_object = get_v_opt<std::map<std::string, std::any>>(value);

        if (opt_object.has_value()) {

            std::map<std::string, std::any> mapObject = opt_object.value(); 
            valueString += "\n" + key + " : \n" + toStringRecursive(mapObject);
        }
    }

    return valueString; 
};

/*
    TODO: Support arrays, not just object
*/