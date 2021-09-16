#include <string>
#include <iostream>
#include <fstream>
#include <utility>
#include "dataParser.h"
#include "jsonParser.h"

JsonParser::JsonParser(const std::string &fileName) {
    data = readFile(fileName);

    result = parsePropsFromData(data);
}

JsonParser::~JsonParser() = default;

std::string JsonParser::readFile(const std::string& fileName) {
    std::string line = "";
    std::fstream file;
    std::string outData;

    bool innString = false;

    file.open(fileName, std::ios::in);

    while (file.is_open() && std::getline(file, line)) {
        if (!line.empty()) {
            for (char c : line) {
                if (!innString) {
                    // Parsing anything else than a string
                    switch (c) {
                        case '"':
                            innString = true;
                        break;
                        case '\t': continue;
                        case '\n': continue;
                        case ' ': continue;
                    }
                } else {
                    if (c == '"') {
                        innString = false;
                    }
                }
                outData += c;
            }
        }
    }

    return outData;
}

DataType JsonParser::parsePropsFromData(std::string string) {
    DataType currentJson;

    //std::cout << string << std::endl;

    for (int i = 0; i <= string.length(); i++) {
        char c = string[i];
        std::string nextSet = getNextSet(string, i);

        if (nextSet.length() == 0)
            continue;

        switch (c) {
            case '{': {
                std::map<std::string, std::string> objects = splitObject(nextSet);

                for (const auto& [key, val] : objects) {
                    //std::cout << key << ": " << val << std::endl;
                    currentJson.add({key, parseValue(val)});
                }

                i += nextSet.length();
                /*

                // std::cout << "o: " << nextSet << std::endl;
                currentJson.add(parseObject(nextSet));

                 */
            }
            break;
            case '[': {
                // std::cout << "a: " << nextSet << std::endl;
                //currentJson.add(parseVector(nextSet));
                std::vector<std::string> objects = splitVector(nextSet);

                for (const std::string& s : objects) {
                    // std::cout << "a: " << s << std::endl;
                    currentJson.add(parseValue(s));
                }

                i += nextSet.length();
            }
            break;
            default: {
                currentJson.add(parseValue(nextSet));

                i += nextSet.length();
            }
            break;
        }
    }

    return currentJson;
}

std::string JsonParser::getNextSet(std::string s, int startIndex) {
    int count = 1;
    char firstChar = s[startIndex];
    char lastChar = endChar[firstChar];
    startIndex++;
    std::string outString = "";

    for (int i = startIndex; i <= s.length(); i++) {
        char c = s[i];

        if (c == firstChar) {
            count++;
        } else if (c == lastChar) {
            count--;
        }

        if (count <= 0) {
            return outString;
        }

        outString += c;
    }

    return outString;
}

std::any JsonParser::parseValue(std::string value) {
    char firstChar = value[0];
    std::any anyVal;

    //std::cout << value << std::endl;

    switch (firstChar) {
        case '{':
            anyVal = std::make_any<DataType>(parsePropsFromData(value));
        break;
        case '[':
            anyVal = std::make_any<DataType>(parsePropsFromData(value));
        break;
        case '"':
            anyVal = std::make_any<std::string>(value.erase(0,1).erase(value.length()-1, 1));
        break;
        default: {
            if (value[value.length()-1] == '}' || value[value.length()-1] == ']')
                value.erase(value.length()-1, 1);
            
            if (DataParser::isBool(value))
                anyVal = std::make_any<bool>(DataParser::parseBool(value));
            if (DataParser::isDouble(value))
                anyVal = std::make_any<double>(DataParser::parseDouble(value));
            if (DataParser::isInt(value))
                anyVal = std::make_any<int>(DataParser::parseInt(value));
        }
    }

    return anyVal;
}

DataType JsonParser::parseObject(std::string value) {
    return parsePropsFromData(std::move(value));
}
/*
std::map<std::string, std::any> JsonParser::parseObject(std::string value) {
    // std::cout << "o: " << value << std::endl;


    return outMap;
}
std::vector<std::any> JsonParser::parseVector(std::string value) {
    // std::cout << "a: " << value << std::endl;



    return outVec;
}
*/
std::vector<std::string> JsonParser::splitVector(const std::string& value) {
    std::vector<std::string> outVec;
    std::string currentValue;

    bool isInString = false;

    for (char c : value) {
        if (c == '"')
            isInString = !isInString;

        if (isInString) {
            currentValue += c;
        } else {
            if (c == ',') {
                outVec.push_back(currentValue);
                currentValue = "";
            } else {
                currentValue += c;
            }
        }
    }

    outVec.push_back(currentValue);

    return outVec;
}

std::map<std::string, std::string> JsonParser::splitObject(std::string value) {
    std::map<std::string, std::string> outObject;
    std::string currentObject;

    int size;

    // Putt this in a while loop (size < totalSize). totalSize = value.size()
    while (value.size() != 0) {
        size_t delimeterIndex = value.find(':');
        std::string left = value.substr(0, delimeterIndex);
        if (left[left.size()-1] == '"' && left[0] != '"') {
            left = '"' + left;
        }

        std::string right = "";

        if (value[delimeterIndex + 1] == '{' || value[delimeterIndex + 1] == '[') {
            char rightChar = value[delimeterIndex + 1];
            right = rightChar + getNextSet(value.erase(0, delimeterIndex + 1), 0) + endChar[rightChar];
        } else {
            std::string nextSet = getNextSet(value.erase(0, delimeterIndex), 0);

            bool isInString = false;
            for (char c : nextSet) {
                if (c == '"') {
                    isInString = !isInString;
                }

                if (isInString) {
                    right += c;
                } else {
                    if (c == ',') {
                        outObject.insert({left, right});
                        break;
                    } else {
                        right += c;
                    }
                }
            }
        }

        outObject.insert({left, right});

        value.erase(0, right.size() + 2);
    }

    return outObject;
}





/*
 * TODO: The parser cant parse "object array advansed"
 * TODO: Object array is weird prints - \n {object}, should be - {object}?
 */
