#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader/tiny_obj_loader.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include "utility/debug.h"
#include "graphics/vertex.h"
#include "graphics/window.h"
#include "graphics/renderer.h"
#include "graphics/model.h"
#include "graphics/shader.h"
#include "graphics/texture.h"
#include "graphics/vertexBuffer.h"
#include "graphics/graphicsEntity.h"
#include "graphics/graphicsEntityInstance.h"
#include "graphics/layoutBinding.h"
#include "graphics/eventManager.h"

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
#include "input/mouse.h"
#include "input/keyboardInput.h"

#include "core/object.h"
#include "core/transform.h"
#include "graphics/camera.h"

#include "rapidxml/rapidxml.hpp"
#include "utility/xml.h"


YamlParser* properties = nullptr;

int main(int argc, char *argv[]) {
    Debug::log(INFO, "Starting application."); 

    // Loading properties file
    std::string filePath = "properties.yaml"; 
    properties = new YamlParser(filePath);

    // Starting debugging
    Debug::setupDebugWindow();

    // Creating a window and attaching a renderer
    Window window;
    RendererContent rendererContent = createRenderer(window);

    // Creating a event manager
    EventManager eventManager;

    // Creating a camera inside a object
    Camera camera(glm::vec3(0.0f, 0.0f, 0.0f), 45.0f, 1920.0f / 1080.0f, 0.1f, 100.0f);
    Object main("Main"); 
    main.addComponent(&camera); 
    
    // Loading a cube model
    Model cubeModel("models/cube.obj");
    
    // Loading a texture 
    Texture cubeTexture = createTexture(rendererContent, "textures/gold.png");

    // Loading a shader
    Shader cubeShader = createShader(rendererContent, "shaders/vert.spv", "shaders/frag.spv");

    // Creating vertex buffer
    VertexBuffer cubeVertexBuffer = createVertexBuffer(rendererContent, cubeModel.vertices, cubeModel.indices);

    // Creating bindings
    std::vector<LayoutBinding> cubeBindings = {{VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER, VK_SHADER_STAGE_VERTEX_BIT, sizeof(UniformBuffer)}, {VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER, VK_SHADER_STAGE_FRAGMENT_BIT}};

    // Creating a graphics entity
    GraphicsEntity cubeEntity = createGraphicsEntity(rendererContent, &cubeVertexBuffer, &cubeTexture, &cubeShader, cubeBindings);
    
    // Creating a cube instance
    GraphicsEntityInstance cubeEntityInstance(rendererContent, &cubeEntity);

    // Creating a transform
    Transform transform({0.0f, 0.0f, 0.0f});

    // Creating a cube object
    Object cube("Cube"); 
    cube.addComponent(&transform); 
    cube.addComponent(&cubeEntityInstance);

    // Setting the mosue in relative mode (mouse dissapears)
    Mouse::enableRelativeMouse();

    // Running rendering loop, this is blocking
    loop(rendererContent, window, eventManager);

    Debug::log("Exiting application!"); 
    SDL_Quit();
    return 0;
}