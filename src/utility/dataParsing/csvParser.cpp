#include <string>
#include <fstream>
#include <iostream>
#include <map>
#include <any>

#include "dataParser.h"
#include "csvParser.h"

CsvParser::CsvParser(const std::string& fileName) {
    lines = readFile(fileName);

    result = parseData();
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
    std::vector<std::vector<std::string>> restData;

    for (int i = 1; i <= lines.size()-1; i++) {
        restData.push_back(DataParser::splitString(lines[i], ','));
    }
    
    DataType outData;

    // std::cout << headers.size() << ", " << restData.size() << std::endl;

    for (int i = 0; i <= headers.size() - 1; i++) {
        std::string header = headers[i];
        
        // std::cout << header << ": ";

        DataType outVec;

        for (int l = 0; l <= restData.size() - 1; l++) {
            // std::cout << restData[l][i] << " ";

            std::any data = restData[l][i];

            outVec.add(data);
        }

        outData.add({header, std::make_any<DataType>(outVec)});

        // std::cout << std::endl;
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
