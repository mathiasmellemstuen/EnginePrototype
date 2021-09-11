#ifndef DATAPARSER_JSONPARSER
#define DATAPARSER_JSONPARSER

#include <string>
#include <map>
#include <vector>
#include <any>

#include "dataParser.h"

class JsonParser : public DataParser {
    public:
        explicit JsonParser(const std::string& fileName);
        ~JsonParser();

        void print();

        DataType operator[](const std::string& key);

    private:
        std::string data;

        std::map<char, char> endChar = {
                {'{', '}'},
                {'[', ']'},
                {'"', '"'}
        };

        std::string readFile(const std::string& fileName);
        DataType parsePropsFromData(std::string string);

        std::string getNextSet(std::string data, int startIndex);

        std::any parseValue(std::string value);
        DataType parseObject(std::string value);
        // std::vector<std::any> parseVector(std::string value);

        std::vector<std::string> splitVector(const std::string& value);
        std::map<std::string, std::string> splitObject(std::string value);
};

#endif //DATAPARSER_JSONPARSER
