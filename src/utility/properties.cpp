#include "properties.h"

#include <map>
#include <string>
#include <fstream>
#include <iostream>
#include "log.h"

Properties properties;

void loadPropertiesFromFile() {

    log(INFO, "Starting loading properties file."); 
    std::fstream propertiesFile; 
    std::string line = ""; 
    std::string delimeter = ":";

    propertiesFile.open("properties.yaml", std::ios::in); 

    while(propertiesFile.is_open() && std::getline(propertiesFile, line)) {
        int delimeterIndex = line.find(delimeter);
        std::string left = line.substr(0, delimeterIndex);
        std::string right = line.erase(0, delimeterIndex + 2);

        if(left.compare(ENGINEPROTOTYPE_TITLE) == 0) {
            properties.title = right;
        } else if(left.compare(ENGINEPROTOTYPE_SCREEN_WIDTH) == 0) {
            properties.screenWidth = (uint32_t)std::stoi(right);
        } else if(left.compare(ENGINEPROTOTYPE_SCREEN_HEIGHT) == 0) {
            properties.screenHeight = (uint32_t)std::stoi(right);
        }
    }

    log(SUCCESS, "Successfully loaded propertiles file."); 
    
};
void printProperties() {
    std::cout << "--- PROPERTIES FILE ---"
    << std::endl << ENGINEPROTOTYPE_TITLE << " : " << properties.title
    << std::endl << ENGINEPROTOTYPE_SCREEN_WIDTH << " : " << properties.screenWidth
    << std::endl << ENGINEPROTOTYPE_SCREEN_HEIGHT << " : " << properties.screenHeight
    << std::endl << "--- END OF PROPERTIES FILE---"
    << std::endl;
};