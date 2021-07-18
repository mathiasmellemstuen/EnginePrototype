#include "graphics/window.h"
#include "graphics/renderer.h"

#include "utility/yamlParser.h"
#include "utility/log.h"
#include "utility/properties.h"
#include <any>
#include <vector>
#include <iostream>
#include <SDL2/SDL.h>

int main(int argc, char *argv[]) {
    log(INFO, "Starting application."); 

    loadPropertiesFromFile();
    printProperties();
    
    YamlParser properties2("prop.yaml");
    log(INFO, properties2.toString());
    
    //YamlField field = properties2["window"]["height"];
    //int i = (int)field; 

    Window window;
    Renderer renderer(window); 

    renderer.loop(); 

    log(INFO, "Exiting application!"); 
    return 0;
}

