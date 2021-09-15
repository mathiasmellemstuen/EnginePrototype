#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <any>

#include "dataParser.h"
#include "csvParser.h"

CsvParser::CsvParser(const std::string& fileName) {
    lines = readFile(fileName);

    /*result = */parseData();
}

CsvParser::~CsvParser(){

}

std::vector<std::string> CsvParser::readFile(const std::string& fileName) {
    std::vector<std::string> outData;

    std::string line = "";
    std::fstream file;

    file.open(fileName, std::ios::in);

    while (file.is_open() && std::getline(file, line)) {
        outData.push_back(line);
    }

    return outData;
}

DataType CsvParser::parseData() {
    std::vector<std::string> headers = DataParser::splitString(lines[0], ',');

    std::vector<std::vector<std::string>> data;

    DataType outData;

    for (int i = 1; i <= lines.size(); i++) {
        data.push_back(splitString(lines[i], ','));
    }

    for (int i = 0; i <= headers.size(); i++) {
        DataType vecData;

        vecData.add(data[i]);

        outData.add({headers[i], vecData});
    }

    return outData;
}

std::vector<int> CsvParser::getDelimeterIndex(const std::string line) {
    std::vector<int> indexes;

    for (int i = 0; i <= line.size(); ++i) {
        char c = line[i];

        if (c == ',') {
            indexes.push_back(i);
        }
    }

   return indexes;
}