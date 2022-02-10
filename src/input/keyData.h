#ifndef ENGINEPROTOTYPE_KEY_DATA
#define ENGINEPROTOTYPE_KEY_DATA

#include <string>

struct KeyData {
    std::string key = "\n";
    bool down = false;
    bool pressed = false; 
    bool up = false; 
};

#endif