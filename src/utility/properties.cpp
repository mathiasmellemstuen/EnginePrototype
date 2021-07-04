#include "properties.h"

#include <map>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>

struct Properties* properties = (struct Properties*)malloc(sizeof(struct Properties));

void loadPropertiesFromFile() {
    
    std::ifstream propertiesFile(ENGINEPROTOTYPE_PROPERTIES_FILE_LOCATION);
    std::string line = ""; 
    std::string delimeter = ":";

    while(std::getline(propertiesFile, line)) {
        int delimeterIndex = line.find(delimeter);
        std::string left = line.substr(0, delimeterIndex);
        std::string right = line.erase(0, delimeterIndex + 2);

        if(left.compare(ENGINEPROTOTYPE_TITLE) == 0) {
            properties->title = right.c_str();
        } else if(left.compare(ENGINEPROTOTYPE_SCREEN_WIDTH) == 0) {
            properties->screenWidth = std::stoi(right);
        } else if(left.compare(ENGINEPROTOTYPE_SCREEN_HEIGHT) == 0) {
            properties->screenHeight = std::stoi(right);
        }
    }
    properties->title = "HELLO";
};
void Properties::print() {
    std::cout << "--- PROPERTIES FILE ---"
    << std::endl << ENGINEPROTOTYPE_TITLE << " : " << title
    << std::endl << ENGINEPROTOTYPE_SCREEN_WIDTH << " : " << screenWidth
    << std::endl << ENGINEPROTOTYPE_SCREEN_HEIGHT << " : " << screenHeight
    << std::endl << "--- END OF PROPERTIES FILE---"
    << std::endl;
}