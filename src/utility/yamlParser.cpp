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

#include <algorithm>

YamlType result;
const char delimeter = ':';
const std::string commnetStart = " #";

std::array<std::string, 3> trueString = { "true", "yes", "on" };
std::array<std::string, 3> falseString = { "false", "no", "off" };

std::vector<std::string> getTabedStrings(std::vector<std::string> lines, int tabLevel, int startLine) {
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

YamlType loadPropFromLines(std::vector<std::string> lines) {
    YamlType currentYaml;

    for (int i = 0; i <= lines.size() - 1; i++) {
        std::string line = lines[i];
        int cTab = getTabLevel(line);
        int nTab = i + 1 <= lines.size() - 1 ? getTabLevel(lines[i + 1]) : -1;
        bool lineIsArray = false;

        log(line);
      
        if (line[cTab + 1] == '-') {
            lineIsArray = true;
        }

        // Finde the index of the delimeter
        std::size_t delimeterIndex = line.find(delimeter);

        if (delimeterIndex == std::string::npos) {  // There is no delimeter in the line, so this value most be a array (- val)!
            std::string val = line.erase(0, cTab + 2);
            currentYaml.vec.push_back(parseValue(val));
        } else {  // The value most be an object (key: val)!
            // Split into key and value
            std::string left = line.substr(0, delimeterIndex).erase(0, cTab);  // split on the delimeterIndex, then remove spaces inform of the key
            std::string right = line.erase(0, delimeterIndex + 2);

            // Check if right is a value or a object (right is "" if it is a object)
            if (right != "") {
                switch (getFirstCharacter(line)) {
                    case '[':
                        currentYaml.map.insert({left, parseInlineVector(line)});
                    break;
                    case '{':
                        currentYaml.map.insert({left, parseInlineObject(line)});
                    break;
                    default:
                        currentYaml.map.insert({left, parseValue(right)});
                    break;
                }             
            } else {
                auto nextTabLines = getTabedStrings(lines, nTab, i + 1);
                auto nextObject = loadPropFromLines(nextTabLines);

                currentYaml.map.insert({left, nextObject});

                i += nextTabLines.size();
            }
        }
    }

    return currentYaml;
}

// Parse the value, then return it as a std::any. This wil store the "true" value in the std::any value
std::any parseValue(std::string line) {
    std::any returnValue;

    char firstCharacter = getFirstCharacter(line);

    // Check if the value is a string ('' or "") or if it is something else
    if (firstCharacter == '\'' || firstCharacter == '"') {  // The value is a string ('' or ""), so remove the quotation mark from the value
        returnValue = std::make_any<std::string>(line.substr(1, line.size() - 2));
        std::cout << line << " - Is a string" << std::endl;
    } else if (isBool(line)) {
        returnValue = std::make_any<bool>(parseBool(line));
        std::cout << line << " - Is a bool = " << parseBool(line) << std::endl;
    }
    
    return returnValue;
}

bool isBool(std::any value) {
    std::string valueString = std::any_cast<std::string>(value);

    // Turn string to lower
    std::transform(valueString.begin(), valueString.end(), valueString.begin(), [](unsigned char c){return std::tolower(c);});

    // Find if valueString is a boolean
    if (std::find(std::begin(trueString), std::end(trueString), valueString) != std::end(trueString)) {
        // valueString is part of the trueString list
        return true;
    } else if (std::find(std::begin(falseString), std::end(falseString), valueString) != std::end(falseString)) {
        // valueString is part of the falseString list 
        return true;
    }

    return false;
}

bool parseBool(std::any value) {
    std::string valueString = std::any_cast<std::string>(value);

    // Turn string to lower
    std::transform(valueString.begin(), valueString.end(), valueString.begin(), [](unsigned char c){return std::tolower(c);});

    // Find if valueString is a boolean
    if (std::find(std::begin(trueString), std::end(trueString), valueString) != std::end(trueString)) {
        // valueString is part of the trueString list
        return true;
    } else if (std::find(std::begin(falseString), std::end(falseString), valueString) != std::end(falseString)) {
        // valueString is part of the falseString list 
        return false;
    }

    return true;
}

std::map<std::string, std::any> parseInlineObject(std::string line) {
    std::map<std::string, std::any> currentMap;

    // Trim the string
    line.erase(0,1);
    line.erase(line.size() - 1);

    auto currentString = splitString(line, ',');

    for (std::string part : currentString) {
        std::size_t delimeterIndex = part.find(delimeter);
        std::string left = part.substr(0, delimeterIndex);
        std::string right = part.erase(0, delimeterIndex + 2);

        currentMap.insert({left, right});
    }

    return currentMap;
}

std::vector<std::any> parseInlineVector(std::string line) {
    std::vector<std::any> currentVec;

    // Trim the string
    line.erase(0,1);
    line.erase(line.size() - 1);

    auto currentString = splitString(line, ',');

    for (std::string part : currentString) {
        currentVec.push_back(part);
    }

    return currentVec;
}

int getTabLevel(std::string line) {
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

char getFirstCharacter(std::string line) {
    char retrunChar;

    for (char c : line) {
        if (c != ' ') {
            return c;
        }
    }

    return ' ';
}

std::vector<std::string> splitString(std::string line, char splitChar) {
    std::stringstream stream(line);
    std::string segment;
    std::vector<std::string> splitString;

    while (std::getline(stream, segment, splitChar)) {
        splitString.push_back(segment);
    }

    return splitString;
}

// Place each line of the file into a vector
std::vector<std::string> readFile(std::string fileName) {
    std::string line = "";
    std::fstream file;
    std::vector<std::string> outLines;
    std::size_t commnetIndex;

    file.open(fileName, std::ios::in);

    while (file.is_open() && std::getline(file, line)) {
        if (!line.empty()) {
            if (line[0] == '#')  // Skip lines that start with a #, because that line is a commnent
                continue;

            // Find the index of " #" because that indicates that there is a comment from that index
            commnetIndex = line.find(commnetStart);

            // If no idex is found then push the entire string, else remove the commnet, then push
            if (commnetIndex == std::string::npos) {
                outLines.push_back(line);
            } else {
                line.erase(commnetIndex);
                outLines.push_back(line);
            }
        }            
    }

    return outLines;
}

/*
    Print thing
*/
// Convert a std::any to a optional, but with a type
template <typename T>
std::optional<T> get_v_opt(const std::any &a) {
    if (const T *v = std::any_cast<T>(&a))
        return std::optional<T>(*v);
    else
        return std::nullopt;
}

std::string buildObjectPrint(std::map<std::string, std::any> object, int tab) {
    std::string outString = "\n";

    for (const auto& [key, value] : object) {
        for (int i = 0; i < tab; i++) {
            outString += "  ";
        }

        outString += key + ": " + buildPrint(value, tab) + "\n";
    }

    return outString;
}

std::string buildVectorPrint(std::vector<std::any> vec, int tab) {
    std::string outString = "\n";

    for (const auto& value : vec) {
        for (int i = 0; i < tab; i++) {
            outString += "  ";
        }

        outString += "- " + buildPrint(value, tab) + "\n";
    }

    return outString;
}

std::string buildYamlTypePrint(YamlType yamlType, int tab) {
    std::string outString = "";

    if (!yamlType.map.empty()) {
        outString += buildObjectPrint(yamlType.map, tab);
    }

    if (!yamlType.vec.empty()) {
        outString += buildVectorPrint(yamlType.vec, tab);
    }

    return outString;
}

// Convert the object to a string, so it can be printed
std::string buildPrint(std::any object, int tab) {
    std::string printString = "";

    // Test if the object is a string
    std::optional opt_string = get_v_opt<std::string>(object);
    if (opt_string.has_value()) {
        return "(std::string) " + opt_string.value();
    }

    // Test if the object is a bool
    std::optional opt_bool = get_v_opt<bool>(object);
    if (opt_bool.has_value()) {
        std::cout << (opt_bool.value() == 0) << std::endl;

        return "(bool)";
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

    std::optional opt_yamlType = get_v_opt<YamlType>(object);
    if (opt_yamlType.has_value()) {
        return buildYamlTypePrint(opt_yamlType.value(), tab + 1);
    }

    return printString;
}

void printResult() {
    std::cout << "--- New prop ---" << std::endl;
    std::string printString = "";    

    for (const auto& [key, value] : result.map) {
        printString += key + ": " + buildPrint(value, 0) + "\n";
    }

    std::cout << printString << std::endl;
    std::cout << "--- End of new prop ---" << std::endl;
}

void runTest() {
    log(INFO, "Started test");

    auto lines = readFile("test.yaml");
    result = loadPropFromLines(lines);

    log(INFO, "Ended test");
}

/*
TODO: Parse value string to correct type
TODO: Make Yaml parcer into a class
TODO?: Refactor some variables name?
*/