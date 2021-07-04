#ifndef ENGINEPROTOTYPE_PROPERTIES
#define ENGINEPROTOTYPE_PROPERTIES

#define ENGINEPROTOTYPE_PROPERTIES_FILE_LOCATION "properties.yaml"
#define ENGINEPROTOTYPE_TITLE "title"
#define ENGINEPROTOTYPE_SCREEN_WIDTH "screenWidth"
#define ENGINEPROTOTYPE_SCREEN_HEIGHT "screenHeight"

#include <map>
#include <string>

struct Properties {
    std::string title; 
    int screenWidth; 
    int screenHeight;

    void print();
};

extern Properties* properties;

void loadPropertiesFromFile();

#endif