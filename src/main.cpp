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

YamlParser* properties = nullptr;

int main(int argc, char *argv[]) {

    Debug::log(INFO, "Starting application."); 
    Debug::log(INFO, "Loading properties.yaml!");

    std::string filePath = "properties.yaml"; 
    properties = new YamlParser(filePath);

    Debug::log(SUCCESS, "Done loading properties.yaml"); 

    Debug::setupDebugWindow(); 
    
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

    Renderer renderer(window);
    Shader shader(renderer, "shaders/vert.spv", "shaders/frag.spv");

    Texture tex(renderer, "textures/viking_room.png");
    Texture tex2(renderer, "textures/texture.png");

    VertexBuffer buffer(renderer, model.vertices, model.indices);
    VertexBuffer buffer2(renderer, verticies, indices);
     
    RenderObject renderObject1(renderer, tex2, shader, buffer2);
    // RenderObject renderObject2(renderer, tex2, shader, buffer2);
    renderObject1.position = glm::vec3(0.0f, 0.0f, -1.0f);
    renderObject1.nextRotationAngles = 3.14f/4;
    glm::mat4 mat1 = renderObject1.createModelMatrix();
    
    renderer.currentRenderObject = &renderObject1;

    renderer.renderFunction = [&](VkCommandBuffer& commandBuffer, int currentCommandBuffer, uint32_t currentImage) {

        renderer.uniformBuffer.update(currentImage, mat1);
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, renderObject1.graphicsPipeline.graphicsPipeline);
        VkBuffer vertexBuffers[] = {renderObject1.vertexBuffer.vertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(commandBuffer,renderObject1.vertexBuffer.indexBuffer, 0, VK_INDEX_TYPE_UINT32);

        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, renderObject1.graphicsPipeline.pipelineLayout, 0, 1, &renderObject1.descriptorPool.descriptorSets[currentCommandBuffer], 0, nullptr);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(renderObject1.vertexBuffer.indices.size()), 1, 0, 0, 0);

    };

    renderer.updateFunction = [&](float deltaTime) {
        
        if(KeyboardInput::keyPressed('w')) {
            renderObject1.position.x -= 0.01f * deltaTime; 
            mat1 = renderObject1.createModelMatrix();
        }

        if(KeyboardInput::keyPressed('s')) {
            renderObject1.position.x += 0.01f * deltaTime; 
            mat1 = renderObject1.createModelMatrix();
        }
        if(KeyboardInput::keyPressed('a')) {
            renderObject1.position.y -= 0.01f * deltaTime; 
            mat1 = renderObject1.createModelMatrix();
        }
        if(KeyboardInput::keyPressed('d')) {
            renderObject1.position.y += 0.01f * deltaTime; 
            mat1 = renderObject1.createModelMatrix();
        }
        if(KeyboardInput::keyPressed('q')) {
            renderObject1.nextRotationAngles += 0.01f * deltaTime;
            mat1 = renderObject1.createModelMatrix();
        }
        if(KeyboardInput::keyPressed('e')) {
            renderObject1.nextRotationAngles -= 0.01f * deltaTime;
            mat1 = renderObject1.createModelMatrix();
        }
    };
    
    renderer.loop(); 
    Debug::log(INFO, "Exiting application!"); 
    return 0;
}
