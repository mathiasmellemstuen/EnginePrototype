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

std::map<std::string, std::any> result;
const char delimeter = ':';

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

std::map<std::string, std::any> loadPropFromLines(std::vector<std::string> lines) {
    std::map<std::string, std::any> currentMap;

    for (int i = 0; i <= lines.size() - 1; i++) {
        std::string line = lines[i];
        int cTab = getTabLevel(line);
        int nTab = 0;
        
        // find the tab level for the next line
        for (char c : lines[i + 1]) {
            if (c == ' ') {
                nTab++;
            } else {
                break;
            }
        }       
        
        // Finde the index of the delimeter
        int delimeterIndex = line.find(delimeter);
        // Split into key and value
        std::string left = line.substr(0, delimeterIndex).erase(0, cTab);  // split on the delimeterIndex, then remove spaces inform of the key
        std::string right = line.erase(0, delimeterIndex + 2);

        // Check if right is a value or a object (right is "" if it is a object)
        if (right != "") {
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

// Place each line of the file into a vector
std::vector<std::string> readFile(std::string fileName) {
    std::string line = "";
    std::fstream file;
    std::vector<std::string> outLines;

    file.open(fileName, std::ios::in);

    while (file.is_open() && std::getline(file, line)) {
        outLines.push_back(line);
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

// Convert the object to a string, so it can be printed
std::string buildPrint(std::any object, int tab) {
    std::string printString = "";

    // Test if the object is a string
    std::optional opt_string = get_v_opt<std::string>(object);
    if (opt_string.has_value()) {
        return "(std::string) " + opt_string.value();
    }
 
    // Test if the object is another object
    std::optional opt_object = get_v_opt<std::map<std::string, std::any>>(object);
    if (opt_object.has_value()) {
        return buildObjectPrint(opt_object.value(), tab + 1);
    }

    return printString;
}

void printResult() {
    std::cout << "--- New prop ---" << std::endl;
    std::string printString = "";    

    for (const auto& [key, value] : result) {
        printString += key + ": " + buildPrint(value, 0) + "\n";
    }

    std::cout << printString << std::endl;
    std::cout << "--- End of new prop ---" << std::endl;
}

void runTest() {
    log(INFO, "Started test");

    auto lines = readFile("prop.yaml");
    result = loadPropFromLines(lines);

    log(INFO, "Ended test");
}

/*
TODO: Support arrays, not just object
TODO: Make Yaml parcer into a class
TODO?: Refactor some variables name?
TODO: Make a findeTabLevel(std::string line){} function
*/