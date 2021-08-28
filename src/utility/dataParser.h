#ifndef ENGINEPROTOTYPE_DATAPARSER
#define ENGINEPROTOTYPE_DATAPARSER

#include <string>
#include <map>
#include <vector>
#include <any>

struct DataType {
    std::string tmpKey;
    int tmpIndex;

    std::map<std::string, std::any> map;
    std::vector<std::any> vec;

    DataType operator[](const char* key);
    DataType operator[](int index);

    operator const int();
    operator const double();
    operator const bool();
    operator const std::string();
    operator const std::map<std::string, std::any>();
    //operator const vector<std::any>();
};

class DataParser {
    public:
        DataType result;
};

#endif