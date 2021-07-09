#ifndef ENGINEPROTOTYPE_PROPERTIES
#define ENGINEPROTOTYPE_PROPERTIES

#define ENGINEPROTOTYPE_TITLE "title"
#define ENGINEPROTOTYPE_SCREEN_WIDTH "screenWidth"
#define ENGINEPROTOTYPE_SCREEN_HEIGHT "screenHeight"
#define ENGINEPrOTOTYPE_MAX_FRAMES_IN_FLIGHT "maxFramesInFlight"

#include <map>
#include <string>

struct Properties {
    std::string title; 
    uint32_t screenWidth; 
    uint32_t screenHeight;
    int maxFramesInFlight; 
};
extern Properties properties;

void loadPropertiesFromFile();
void printProperties(); 
#endif