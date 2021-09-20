#ifndef ENGINEPROTOTYPE_DATATYPE
#define ENGINEPROTOTYPE_DATATYPE

#include <string>
#include <map>
#include <vector>
#include <any>
#include <utility>  // std::pair

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

    void add(std::pair<std::string, std::any> pair);
    // void add(std::map<std::string, std::any> value);
    void add(const std::any& val);
};

#endif