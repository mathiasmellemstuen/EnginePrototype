#define TINYOBJLOADER_IMPLEMENTATION
#include <tinyobjloader/tiny_obj_loader.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb/stb_image.h>

#include <ft2build.h>
#include FT_FREETYPE_H

#include "rapidxml/rapidxml.hpp"

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
#include "graphics/camera.h"

#include "graphics/UI/UIInstance.h"
#include "graphics/UI/UIRectangleInstance.h"
#include "graphics/UI/UITriangleInstance.h"
#include "graphics/UI/UICircleInstance.h"
#include "graphics/UI/UITextInstance.h"

#include "input/mouse.h"
#include "input/keyboardInput.h"

#include "core/object.h"
#include "core/transform.h"
#include "core/predefined.h"

#include "utility/xml.h"
#include "utility/properties.h"
#include "utility/debug.h"
#include "utility/logging.h"

#include "graphics/text/text.h"

YamlParser* properties = nullptr;

int main(int argc, char *argv[]) {
    logger(INFO, "Starting application2."); 

    // Loading properties file
    std::string filePath = "properties.yaml"; 
    properties = new YamlParser(filePath);
    

    #ifndef NO_DEBUG_WINDOW
    // Starting debugging
    logger(INFO, "Starting debugging!");
    Debug::setupDebugWindow();
    #endif


    // Creating a window and attaching a renderer
    Window window;
    RendererContent rendererContent = createRenderer(window);

    createCharacterMapFromFont(rendererContent, "assets/fonts/Roboto-Black.ttf");

    // Loading engine spesific predefined content
    loadPredefined(rendererContent);

    // Creating a event manager
    EventManager eventManager;

    // Creating a camera inside a object
    Camera camera(glm::vec3(-10.0f, 0.0f, 2.0f), 45.0f, 1920.0f / 1080.0f, 0.1f, 100.0f);
    Object main("Main"); 
    main.addComponent(&camera); 
    
    // Loading a cube model
    Model cubeModel("assets/models/cube.obj");
    
    // Loading a texture 
    Texture cubeTexture = createTexture(rendererContent, "assets/textures/gold.png");

    // Loading a shader
    Shader cubeShader = createShader(rendererContent, "assets/shaders/compiled/default.vert.spv", "assets/shaders/compiled/default.frag.spv");

    // Creating vertex buffer
    VertexBuffer cubeVertexBuffer = createVertexBuffer(rendererContent, cubeModel.vertices, cubeModel.indices);

    // Creating a graphics entity
    GraphicsEntity cubeEntity = createGraphicsEntity(rendererContent, &cubeShader, &cubeVertexBuffer, &cubeTexture);
    
    // Creating a cube instance
    GraphicsEntityInstance<UniformBufferObject> cubeEntityInstance(rendererContent, &cubeEntity);

    // Creating a transform
    Transform transform({0.0f, 0.0f, 0.0f});

    // Creating a cube object
    Object cube("Cube");
    cube.addComponent(&transform);
    cube.addComponent(&cubeEntityInstance);
    
    Shader uiShader = createShader(rendererContent, "assets/shaders/compiled/uiShader.vert.spv", "assets/shaders/compiled/uiShader.frag.spv");

    // Creating a UI triangle
    UITriangleInstance triangle1(rendererContent); 
    triangle1.position = {0.8, 0.0};
    triangle1.color = {1.0, 0, 0, 0.5};
    triangle1.size = {0.2, 0.2};
    triangle1.layer = 0;
    Object uiTriangle("Triangle Object"); 
    uiTriangle.addComponent(&triangle1);

    // Creating a UI rectangle
    UIRectangleInstance uiRectangleInstance(rendererContent); 
    uiRectangleInstance.position = {-0.7, 0.0};
    uiRectangleInstance.color = {0.0, 1.0, 0.0, 0.7};
    uiRectangleInstance.size = {0.1, 0.1};
    uiRectangleInstance.layer = 1; 
    Object uiRectangle("UI Rectangle Object"); 
    uiRectangle.addComponent(&uiRectangleInstance); 

    // Creating a UI circle
    UICircleInstance uiCircleInstance(rendererContent);
    uiCircleInstance.position = {0.0, 0.0};
    uiCircleInstance.color = {0.0, 0.0, 1.0, 0.6};
    uiCircleInstance.setRadius(0.4f); 
    Object uiCirlce("UI Cirlce Object"); 
    uiCirlce.addComponent(&uiCircleInstance);

    // Creating a text
    UITextInstance uiTextInstance(rendererContent);
    uiTextInstance.position = {0.5, 0.0};
    uiTextInstance.color = {0.0, 0.0, 1.0, 1.0};
    uiTextInstance.size = {0.8, 0.8};
    Object uiTextObj("UI text object");
    uiTextObj.addComponent(&uiTextInstance);

    // Setting the mosue in relative mode (mouse dissapears)
    Mouse::enableRelativeMouse();
    
    logger(INFO, "Starting rendering loop!");
    
    // Running rendering loop, this is blocking
    loop(rendererContent, window, eventManager);

    logger(INFO, "Exiting application!"); 
    SDL_Quit();
    return 0;
}