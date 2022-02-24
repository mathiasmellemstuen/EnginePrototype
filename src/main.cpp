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
#include "input/mouse.h"
#include "input/keyboardInput.h"

#include "core/object.h"
#include "core/transform.h"
#include "graphics/camera.h"

#include "rapidxml/rapidxml.hpp"


YamlParser* properties = nullptr;

int main(int argc, char *argv[]) {

    Debug::log(INFO, "Starting application."); 
    Debug::log(INFO, "Loading properties.yaml!");

    std::string filePath = "properties.yaml"; 
    properties = new YamlParser(filePath);

    Debug::log(SUCCESS, "Done loading properties.yaml"); 

    Debug::setupDebugWindow();

    Window window;

    Renderer renderer(window);

    // Loading all the assets for a cube and creating the cube object
    Model cubeModel("models/cube.obj");
    Shader shader(renderer, "shaders/vert.spv", "shaders/frag.spv");
    Shader colorShader(renderer, "shaders/colorShader.vert.spv", "shaders/colorShader.frag.spv");
    Texture goldTexture(renderer, "textures/gold.png");
    VertexBuffer buffer(renderer, cubeModel.vertices, cubeModel.indices);
    RenderObject cubeRender(renderer, goldTexture, shader, buffer);
    RenderObject cube2Render(renderer, goldTexture, colorShader, buffer);

    Object cube("Cube", &cubeRender);
    cube.renderInstance.transform.position = glm::vec3(0.0f, 2.0f, 4.0f);

    // Creating another identical cube and moving it to another position
    Object cube2("Cube2", &cube2Render);
    cube2.renderInstance.transform.position = glm::vec3(0.0f, 5.0f, 0.0f);

    // Creating another identical cube and moving it to another position
    Object cube3("Cube3", &cube2Render);
    cube3.renderInstance.transform.position = glm::vec3(0.0f, -5.0f, 0.0f);

    // Creating another identical cube and moving it to another position
    Object cube4("Cube4", &cube2Render);
    cube4.renderInstance.transform.position = glm::vec3(-5.0f, -5.0f, 0.0f);

    // Creating another identical cube and moving it to another position
    Object cube5("Cube5", &cube2Render);
    cube5.renderInstance.transform.position = glm::vec3(-5.0f, 5.0f, 0.0f);

    // Rapidxml paring testing
    Model testModel("Man", Model::DAE);
    Texture testTexture(renderer, "textures/ManTex.jpg");
    VertexBuffer buffer2(renderer, testModel.vertices, testModel.indices);
    RenderObject testRender(renderer, testTexture, shader, buffer2);
    Object tModel("Test Model", &testRender);
    // end

    Camera camera(glm::vec3(-5.0f, 0.0f, 0.0f), 45.0f, 1920.0f / 1080.0f, 0.1f, 100.0f);

    Mouse::enableRelativeMouse();

    renderer.loop();

    //cubeRender.descriptorPool.clean();
    cubeRender.descriptorSetLayout.clean(); 
    cubeRender.shader.clean(); 
    cubeRender.texture.clean();
    cubeRender.vertexBuffer.clean();
    renderer.cleanupSwapChain();
    renderer.syncObjects.clean(); 
    renderer.commandPool.clean();
    renderer.logicalDevice.clean(); 
    renderer.vulkanInstance.clean(); 
    window.clean();
    Debug::log("Exiting application!"); 
    SDL_Quit();
    return 0;
}