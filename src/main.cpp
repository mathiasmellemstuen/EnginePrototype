#define TINYOBJLOADER_IMPLEMENTATION
#include <tiny_obj_loader.h>

#define STB_IMAGE_IMPLEMENTATION
#include <stb_image.h>

#include "graphics/vertex.h"
#include "graphics/window.h"
#include "graphics/renderer.h"
#include "graphics/vertex.h"
#include "graphics/model.h"
#include "graphics/shader.h"
#include "graphics/rendererInfo.h"
#include "graphics/texture.h"
#include "graphics/vertexBuffer.h"

#include "utility/dataParsing/yamlParser.h"
#include "utility/dataParsing/jsonParser.h"
#include "utility/dataParsing/csvParser.h"
#include "utility/debug.h"

#include <any>
#include <vector>
#include <iostream>
#include <SDL2/SDL.h>
#include <functional>
#include <glm/glm.hpp>

int main(int argc, char *argv[]) {

    Debug::log(INFO, "Starting application."); 
    Debug::setupDebugWindow(); 

    {// Yaml parsing test
        std::cout << "*** YamlParser ***" << std::endl;

        YamlParser parser("Test_data/test.yaml");
        
        std::any string = parser.result.map["test types"];
        
        std::cout << string.type().name() << std::endl;


        /*
        int i = parser["test types"]["base"]["desimal"];
        std::cout << "i = " << i << std::endl;
        */

        std::cout << "*** YamlParser ***" << std::endl;
    }

    {// Json parser test
        std::cout << "*** JsonParser ***" << std::endl;

        // JsonParser parser("Test_data/test.json");

        std::cout << "*** JsonParser ***" << std::endl;
    }

    {// CSV parser test
        std::cout << "*** CsvParser ***" << std::endl;
        
        CsvParser parser("Test_data/test.csv");
        
        std::any string = parser.result.map["d1"];

        std::cout << string.type().name() << std::endl;

        std::cout << "*** CsvParser ***" << std::endl;
    }


    

    

//    setup();
    
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

    RendererInfo rendererInfo1(renderer, tex, shader, buffer);
    RendererInfo renderInfo2(renderer, tex2, shader, buffer2);
    rendererInfo1.position = glm::vec3(0.0f, 0.0f, -1.0f);
    renderInfo2.position = glm::vec3(0.0f, 0.0f, -1.0f); 
    glm::mat4 mat2 = renderInfo2.createModelMatrix();
    glm::mat4 mat1 = rendererInfo1.createModelMatrix();

    renderer.currentRendererInfo = &rendererInfo1;

    renderer.updateFunction = [&](VkCommandBuffer& commandBuffer, int currentCommandBuffer, uint32_t currentImage) {

        renderer.uniformBuffer.update(currentImage, mat2);
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, rendererInfo1.graphicsPipeline.graphicsPipeline);
        VkBuffer vertexBuffers2[] = {rendererInfo1.vertexBuffer.vertexBuffer};
        VkDeviceSize offsets2[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers2, offsets2);
        vkCmdBindIndexBuffer(commandBuffer,rendererInfo1.vertexBuffer.indexBuffer, 0, VK_INDEX_TYPE_UINT32);

        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, rendererInfo1.graphicsPipeline.pipelineLayout, 0, 1, &rendererInfo1.descriptorPool.descriptorSets[currentCommandBuffer], 0, nullptr);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(rendererInfo1.vertexBuffer.indices.size()), 1, 0, 0, 0);

    };
    
    renderer.loop(); 
    Debug::log(INFO, "Exiting application!"); 
    return 0;
}
