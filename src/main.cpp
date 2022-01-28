#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader/tiny_obj_loader.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "utility/debug.h"
#include "graphics/vertex.h"
#include "graphics/window.h"
#include "graphics/renderer.h"
#include "graphics/vertex.h"
#include "graphics/model.h"
#include "graphics/shader.h"
#include "graphics/renderObject.h"
#include "graphics/texture.h"
#include "graphics/vertexBuffer.h"

#include "cpp-data-parsing/yaml/yamlParser.h"
#include "cpp-data-parsing/json/jsonParser.h"
#include "cpp-data-parsing/csv/csvParser.h"

#include <any>
#include <vector>
#include <iostream>
#include <SDL2/SDL.h>
#include <functional>
#include <glm/glm.hpp>
#include <string>

#include "utility/properties.h"
#include "input/mouseInput.h"
#include "input/keyboardInput.h"

#include "core/object.h"
#include "core/transform.h"

YamlParser* properties = nullptr;

int main(int argc, char *argv[]) {

    Debug::log(INFO, "Starting application."); 
    Debug::log(INFO, "Loading properties.yaml!");

    std::string filePath = "properties.yaml"; 
    properties = new YamlParser(filePath);

    Debug::log(SUCCESS, "Done loading properties.yaml"); 

    Debug::setupDebugWindow(); 
    
    std::vector<Vertex> verticies = {
        {{-0.5f, -0.5f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.4f}},
        {{0.5f, -0.5f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.4f}},
        {{0.5f, 0.5f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},

        {{-0.5f, -0.5f, -0.5f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{0.5f, -0.5f, -0.5f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{0.5f, 0.5f, -0.5f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}},
        {{-0.5f, 0.5f, -0.5f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}}
    };

    std::vector<uint32_t> indices = {
        0, 1, 3, 3, 1, 2,
        1, 5, 2, 2, 5, 6,
        5, 4, 6, 6, 4, 7,
        4, 0, 7, 7, 0, 3,
        3, 2, 7, 7, 2, 6,
        4, 5, 0, 0, 5, 1
    };

    Model model("models/viking_room.obj");
    Window window;

    Renderer renderer(window);
    Shader shader(renderer, "shaders/vert.spv", "shaders/frag.spv");

    Texture tex(renderer, "textures/viking_room.png");
    Texture tex2(renderer, "textures/philosophers.png");

    VertexBuffer buffer(renderer, model.vertices, model.indices);
    VertexBuffer buffer2(renderer, verticies, indices);

    std::string name = "Cube";
    Transform transform;
    RenderObject cubeRender(renderer, tex2, shader, buffer2);

    Object cube(name, transform, cubeRender);
    std::function<void(float& deltaTime)> u = [&](float& deltaTime) {
        
        Debug::log("Something else"); 
        if(KeyboardInput::keyPressed('w')) {
            cube.transform.position.x -= 0.01f * deltaTime; 
        }

        if(KeyboardInput::keyPressed('s')) {
            cube.transform.position.x += 0.01f * deltaTime; 
        }
        if(KeyboardInput::keyPressed('a')) {
            cube.transform.position.y -= 0.01f * deltaTime; 
        }
        if(KeyboardInput::keyPressed('d')) {
            cube.transform.position.y += 0.01f * deltaTime; 
        }
        if(KeyboardInput::keyPressed('q')) {
            cube.transform.angle += 0.01f * deltaTime;
        }
        if(KeyboardInput::keyPressed('e')) {
            cube.transform.angle -= 0.01f * deltaTime;
        }
    };
    cube.setUpdate(u); 
    renderer.loop(); 
    Debug::log(INFO, "Exiting application!"); 
    return 0;
}