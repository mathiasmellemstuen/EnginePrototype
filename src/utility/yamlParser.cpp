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

/*
Yaml Type
*/
// Adding data to the array
void YamlType::add(std::any val) {
    if (type == ARRAY) {
        std::vector<std::any> vec = std::any_cast<std::vector<std::any>>(data);
        vec.push_back(val);

        data = vec;
    }
}

// Adding data to the map
void YamlType::add(std::pair<std::string, std::any> val) {
    if (type == MAP) {
        std::map<std::string, std::any> map = std::any_cast<std::map<std::string, std::any>>(data);
        map.insert(val);

        data = map;
    }
}

/*
Yaml Parser
*/
YamlParser::YamlParser(const std::string fileName) {
    lines = readFile(fileName);
    result = parsePropsFormLines(lines);
}

YamlParser::~YamlParser() {

};

std::vector<std::string> YamlParser::readFile(const std::string fileName) {
    std::string line = "";
    std::fstream file;
    std::vector<std::string> outLines;
    std::size_t commnetIndex;

    bool isReading = false;

    file.open(fileName, std::ios::in);

    while (file.is_open() && std::getline(file, line)) {
        if (line.compare(0, 3, fileStart) == 0) {
            log("Start Parsing");
            isReading = true;
            continue;
        } else if (line.compare(0, 3, fileEnd) == 0) {
            log("End Parsing");
            return outLines;
        }

        if (!line.empty() && isReading) {
            if (line[0] == commentChar)  // Skip lines that start with a #, because that line is a commnent
                continue;

            // Find the index of " #" because that indicates that there is a comment from that index
            commnetIndex = line.find(commentChar + ' ');

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
Parsing
*/
YamlType YamlParser::parsePropsFormLines(std::vector<std::string> lines) {
    YamlType currentYaml;

    for (int i = 0; i <= lines.size() - 1; i++) {
        int cTab = getTabLevel(lines[i]);
        int nTab = i + 1 <= lines.size() - 1 ? getTabLevel(lines[i + 1]) : -1;
        std::string line = lines[i].erase(0, cTab);

        if (currentYaml.type == NULL) {
            currentYaml.type = getType(line);
            switch (currentYaml.type) {
                case ARRAY:
                    currentYaml.data = std::any_cast<std::vector<std::any>>(currentYaml.data);
                break;
                case MAP:
                    currentYaml.data = std::any_cast<std::map<std::string, std::any>>(currentYaml.data);
                break;
            }
        }

        switch (currentYaml.type) {
            case ARRAY: {
                std::string val = line.erase(0, 2);
                currentYaml.add(parseValue(val));
                break;
            }
            case MAP: {
                std::size_t delimeterIndex = line.find(mapChar + ' ');

                std::string left = line.substr(0, delimeterIndex);
                std::string right = line.erase(0, delimeterIndex + 2);

                if (right != "") {
                    switch (getFirstChar(line)) {
                        case '[':
                            currentYaml.add({left, parseInlineVector(right)});
                        break;
                        case '{':
                            currentYaml.add({left, parseInlineObject(right)});
                        break;
                        default:
                            currentYaml.add({left, parseValue(line)});
                        break;
                    }
                } else {
                    auto nextTabLines = getTabedString(lines, nTab, i + 1);
                    auto nextObject = parsePropsFormLines(nextTabLines);

                    currentYaml.add({left, nextObject});

                    i += nextTabLines.size();
                }
                break;
            }       
        }


        /*
        // Finde the index of the delimeter
        std::size_t delimeterIndex = line.find(mapChar + ' ');

        if (delimeterIndex == std::string::npos) {  // There is no delimeter in the line, so this value most be a array (- val)!
            std::string val = line.erase(0, cTab + 2);
            currentYaml.vec.push_back(parseValue(val));
        } else {  // The value most be an object (key: val)!
            // Split into key and value
            std::string left = line.substr(0, delimeterIndex).erase(0, cTab);  // split on the delimeterIndex, then remove spaces inform of the key
            std::string right = line.erase(0, delimeterIndex + 2);

            // Check if right is a value or a object (right is "" if it is a object)
            if (right != "") {
                switch (getFirstChar(line)) {
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
        */
    }

    return currentYaml;
}

std::any YamlParser::parseValue(std::string value) {
    std::any returnValue;

    char firstCharacter = getFirstChar(value);

    // Check if the value is a string ('' or "") or if it is something else
    if (firstCharacter == '\'' || firstCharacter == '"') {  // The value is a string ('' or ""), so remove the quotation mark from the value
        returnValue = std::make_any<std::string>(value.substr(1, value.size() - 2));
        std::cout << value << " - Is a string" << std::endl;
    } else if (isBool(value)) {
        returnValue = std::make_any<bool>(parseBool(value));
        std::cout << value << " - Is a bool = " << parseBool(value) << std::endl;
    }
    
    return returnValue;
}

std::map<std::string, std::any> YamlParser::parseInlineObject(std::string object) {
    std::map<std::string, std::any> currentMap;

    // Trim the string
    object.erase(0,1);
    object.erase(object.size() - 1);

    auto currentString = splitString(object, listSplitChar + ' ');

    for (std::string part : currentString) {
        std::size_t delimeterIndex = part.find(mapChar);
        std::string left = part.substr(0, delimeterIndex);
        std::string right = part.erase(0, delimeterIndex + 2);

        currentMap.insert({left, right});
    }

    return currentMap;
}

std::vector<std::any> YamlParser::parseInlineVector(std::string vector) {
    std::vector<std::any> currentVec;

    // Trim the string
    vector.erase(0,1);
    vector.erase(vector.size() - 1);

    auto currentString = splitString(vector, listSplitChar + ' ');

    for (std::string part : currentString) {
        currentVec.push_back(part);
    }

    return currentVec;
}

bool YamlParser::parseBool(std::string value) {
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

/*
Utility
*/
char YamlParser::getFirstChar(std::string line) {
    for (char c : line)
        if (c != ' ')
            return c;
    return ' ';   
}

bool YamlParser::isBool(std::string value) {
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

std::vector<std::string> YamlParser::splitString(std::string line, char splitChar) {
    std::stringstream stream(line);
    std::string segment;
    std::vector<std::string> splitString;

    while (std::getline(stream, segment, splitChar)) {
        splitString.push_back(segment);
    }

    return splitString;   
}

int YamlParser::getTabLevel(std::string line) {
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

Types YamlParser::getType(std::string line) {
    if (line[0] == arrayChar) {
        return ARRAY;
    } else {
        return MAP;
    }
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
template <typename T>
std::optional<T> YamlParser::get_v_opt(const std::any &a) {
    if (const T *v = std::any_cast<T>(&a))
        return std::optional<T>(*v);
    else
        return std::nullopt;
}

std::string YamlParser::buildPrint(std::any object, int tab) {
    std::string printString = "";

    // Test if the object is a string
    //std::optional opt_string = get_v_opt<std::string>(object);
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

std::string YamlParser::buildObjectPrint(std::map<std::string, std::any> object, int tab) {
    std::string outString = "\n";

    for (const auto& [key, value] : object) {
        for (int i = 0; i < tab; i++) {
            outString += "  ";
        }

        outString += key + ": " + buildPrint(value, tab) + "\n";
    }

    return outString;
}

std::string YamlParser::buildVectorPrint(std::vector<std::any> vec, int tab) {
    std::string outString = "\n";

    for (const auto& value : vec) {
        for (int i = 0; i < tab; i++) {
            outString += "  ";
        }

        outString += "- " + buildPrint(value, tab) + "\n";
    }

    return outString;
}

std::string YamlParser::buildYamlTypePrint(YamlType yamlType, int tab) {
    std::string outString = "";

    switch (yamlType.type) {
        case ARRAY:
            return buildVectorPrint(std::any_cast<std::vector<std::any>>(yamlType.data), tab);
        case MAP:
            return buildObjectPrint(std::any_cast<std::map<std::string, std::any>>(yamlType.data), tab);
    }

    return outString;
}

void YamlParser::print() {
    std::cout << "--- New prop ---" << std::endl;
    std::string printString = "";

    if (result.type == ARRAY) {
        std::vector<std::any> vec = std::any_cast<std::vector<std::any>>(result.data);

        for (const auto& value : vec) {
            printString += arrayChar + " " + buildPrint(vec, 0) + "\n";
        }
    } else if (result.type == MAP) {
        std::map<std::string, std::any> map = std::any_cast<std::map<std::string, std::any>>(result.data);

        for (const auto& [key, value] : map) {
            printString += key + mapChar + " " + buildPrint(map, 0) + "\n";
        }
    }

    std::cout << printString << std::endl;
    std::cout << "--- End of new prop ---" << std::endl;
}

/*
YamlField YamlField::operator[](const std::string& str) {
    std::optional optionalMap = std::any_cast<std::map<std::string, std::any>>(*data)[str];

    if(optionalMap.has_value()) {
        log(INFO, "Returning a any cast to a map."); 
        log(std::any_cast<std::string>(std::any_cast<std::map<std::string, std::any>>(*data)["title"]));
        return { &std::any_cast<std::map<std::string, std::any>>(*data)[str] };
    }
    
    log(INFO, "Returning something else instead."); 
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
    std::string value = "";

    for(const auto& [key, value] : obj) {

        // Test if the object is a string
        std::optional opt_string = get_v_opt<std::string>(value);
        if (opt_string.has_value()) {
            value += "\n" + key + " : " + opt_string.value();
        }

        // Test if the object is another object
        std::optional opt_object = get_v_opt<std::map<std::string, std::any>>(value);

        if (opt_object.has_value()) {

            std::map<std::string, std::any> mapObject = opt_object.value(); 
            value += "\n" + key + " : \n" + toStringRecursive(mapObject);
        }
    }

    return value; 
};
*/
/*
    TODO: Support arrays, not just object
    TODO: Support start and end tags for YAML files. (---) for start and (...) for end
    TODO: Make YAML parser class
    TODO: Parse value string into correct type
*/