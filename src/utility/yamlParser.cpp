#include "yamlParser.h"

#include <string>
#include <map>
#include <any>
#include <fstream>
#include <iostream>
#include <vector>
#include <sstream>
#include <optional>
#include <typeinfo>

#include "log.h"

std::map<std::string, std::any> result;

char delimeter = ':';
int numSpaceForTab = 2;
int debugInt = 0;

std::vector<std::string> getTabLevel(std::vector<std::string> lines, int tabLevel, int startLine) {
    std::vector<std::string> returnString;
    std::string line = "";

    for (int i = startLine; i < lines.size(); i++) {
        line = lines[i];
        int cTab = 0;

        // Finde the tab level for the current line
        for (int l = 0; l < INT_MAX; l++) {
            if (line[l] == ' ') {
                cTab++;
            } else {
                break;
            }
        }

        if (cTab >= tabLevel) {
            returnString.push_back(line);
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
        int cTab = 0;
        int nTab = 0;
        
        // Finde the tab level for the current line
        for (char c : line) {
            if (c == ' ') {
                cTab++;
            } else {
                break;
            }
        }

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
            auto nextTabLines = getTabLevel(lines, nTab, i + 1);
            auto nextObject = loadPropFromLines(nextTabLines);

            currentMap.insert({left, nextObject});

            i += nextTabLines.size();
        }
    }

    return currentMap;
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

// Convert a std::any to a optional, but with a type
template <typename T>
std::optional<T> get_v_opt(const std::any &a) {
    if (const T *v = std::any_cast<T>(&a))
        return std::optional<T>(*v);
    else
        return std::nullopt;
}

/*
    Print thing
*/
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

    // Test if the object is a int
    std::optional opt_int = get_v_opt<int>(object);
    if (opt_int.has_value()) {
        return  "" + opt_int.value();
    }
    
    // Test if the object is a string
    std::optional opt_string = get_v_opt<std::string>(object);
    if (opt_string.has_value()) {
        return opt_string.value();
    }
 
    // Test if the object is another object
    std::optional opt_object = get_v_opt<std::map<std::string, std::any>>(object);
    if (opt_object.has_value()) {
        printString = buildObjectPrint(opt_object.value(), tab + 1);
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