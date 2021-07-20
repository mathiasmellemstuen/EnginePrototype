#include "graphics/window.h"
#include "graphics/renderer.h"
#include "graphics/vertex.h"

#include "utility/yamlParser.h"
#include "utility/log.h"
#include <any>
#include <vector>
#include <iostream>
#include <SDL2/SDL.h>

int main(int argc, char *argv[]) {
    log(INFO, "Starting application."); 

    //YamlParser properties2("prop.yaml");
    //log(INFO, properties2.toString());
    
    //YamlField field = properties2["window"]["height"];
    //int i = (int)field; 

    std::vector<Vertex> verticies = {
        {{0.0f, -0.5f}, {1.0f, 0.0f, 0.0f}},
        {{0.5f, 0.5f}, {0.0f, 1.0f, 0.0f}},
        {{-0.5f, 0.5f}, {0.0f, 0.0f, 1.0f}}
    };

    Window window;
    Renderer renderer(window, verticies); 

    renderer.loop(); 

    log(INFO, "Exiting application!"); 
    return 0;
}

