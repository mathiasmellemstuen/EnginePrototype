#include "graphics/vertex.h"

#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "graphics/window.h"
#include "graphics/renderer.h"
#include "graphics/vertex.h"
#include "graphics/model.h"
#include "utility/yamlParser.h"
#include "utility/debug.h"

#include <any>
#include <vector>
#include <iostream>
#include <SDL2/SDL.h>

int main(int argc, char *argv[]) {



    Debug::log(INFO, "Starting application."); 

    //YamlParser properties2("prop.yaml");
    //log(INFO, properties2.toString());
    
    //YamlField field = properties2["window"]["height"];
    //int i = (int)field; 

    //YamlParser parser("test.yaml");
    //parser.print();

     std::vector<Vertex> verticies = {
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
    };

    std::vector<uint32_t> indices = {
        0, 1, 2, 2, 3, 0,
        4, 5, 6, 6, 7, 4
    };

    Model model("models/viking_room.obj");
    Window window;
    Renderer renderer(window, model.vertices, model.indices); 

    renderer.loop(); 

    Debug::log(INFO, "Exiting application!"); 
    return 0;
}

