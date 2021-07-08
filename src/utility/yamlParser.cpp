#include "yamlParser.h"

#include <string>
#include <map>
#include <any>
#include <fstream>
#include <iostream>

#include <sstream>

#include <optional>

#include <algorithm>
#include <typeinfo>

#include "log.h"

std::map<std::string, std::any> result;

char delimeter = ':';
int numSpaceForTab = 2;

std::map<std::string, std::any> loadPropFromFile(int spaceNr, int& lineNr, std::fstream& file) {
    log(INFO, "Started new load func");
    log(INFO, spaceNr);

    std::map<std::string, std::any> currentProp;
    std::string line = "";  //TODO: I think the problem (does not read afther a object) is here?

    while (file.is_open() && std::getline(file, line)) {
        int numOfSpaces;  // Numer of scases in front of a key

        // Test if the first character is a space
        if (line[0] == 32) {  // 32 == Askii for space
            // Loop thrue all spaces to funde the tab level
            for (int i = 0; i <= INT_MAX; i++) {
                if (line[i] == 32)
                    numOfSpaces++;
                else 
                    break; // Break if not found a space
            }

            if (numOfSpaces < spaceNr) {  // Return if going a tab level down
                return currentProp;
            }
        }

        int delimeterIndex = line.find(delimeter);

        // Split into key and value
        std::string left = line.substr(0, delimeterIndex).erase(0, spaceNr);  // split on the delimeterIndex, then remove spaces inform of the key
        std::string right = line.erase(0, delimeterIndex + 2);

        log(ERROR, left);

        // Check if right is a value or a object (right is "" if it is a object)
        if (right != "") {
            currentProp.insert({left, right});
        } else {
            currentProp.insert({left, loadPropFromFile(spaceNr + numSpaceForTab, lineNr, file)});
        }
    }

    log(INFO, "Ended load func");
    log(INFO, spaceNr);
    return currentProp;
}

void runTest() {
    log(INFO, "started test");

    std::fstream file;
    file.open("prop.yaml", std::ios::in);

    int spaceNum = 0;
    int lineNum = 0;

    result = loadPropFromFile(spaceNum, lineNum, file);

    /*
    std::for_each(result.begin(), result.end(), [](const std::pair<std::string, std::any> &p) {
        //log(ERROR, p.first + " - " + typeid(p.second).name());
        log(ERROR, p.first + " - " + p.second.type().name());
    });
    */

    log(INFO, "Ended test");
}

// Convert a std::any to a optional, but with a type
template <typename T>
std::optional<T> get_v_opt(const std::any &a) {
    if (const T *v = std::any_cast<T>(&a))
        return std::optional<T>(*v);
    else
        return std::nullopt;
}

// Convert the object to a string, so it can be printed
std::string buildPrint(std::any object) {
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
 
    // Test if the object is a nother object
    std::optional opt_object = get_v_opt<std::map<std::string, std::any>>(object);
    if (opt_object.has_value()) {
        printString = "is object";
    }

    return printString;
}

/*
std::string buildPrint(std::any object) {
    std::string printString = "";

    std::for_each(result.begin(), result.end(), [printString](const std::pair<std::string, std::any> &p) {
        //printString = p.first + ": ";

        

        std::optional opt_int = get_v_opt<int>(p.second);
        if (opt_int.has_value())
            std::cout << opt_int.value();

        std::optional opt_string = get_v_opt<std::string>(p.second);
        if (opt_string.has_value())    
            std::cout << opt_string.value();

        std::optional opt_object = get_v_opt<std::map<std::string, std::any>>(p.second);
        if (opt_object.has_value())
            std::cout << "is object";

        std::cout << std::endl;

    });

    return printString;
}
*/
void printResult() {
    std::cout << "--- New prop ---" << std::endl;
    
    std::for_each(result.begin(), result.end(), [](const std::pair<std::string, std::any> &p) {
        std::cout << p.first << ": " << buildPrint(p.second) << std::endl;
    });

    std::cout << "--- End of new prop ---" << std::endl;
}

/*
TODO: Fix not reading object afther the first object
*/