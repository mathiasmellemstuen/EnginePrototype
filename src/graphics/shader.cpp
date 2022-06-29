#include "shader.h"

#include <vulkan/vulkan.h>
#include <fstream>
#include <vector>
#include <string>
#include "../utility/logging.h"
#include <iostream>
#include "renderer.h"

const Shader& createShader(RendererContent& rendererContent, std::string vertexShaderPath, std::string fragmentShaderPath, std::string geometryShaderPath) {
    Shader* shader = new Shader; 
    shader->shaderCount = 2; 

    logger(INFO, "Creating shader"); 

    auto vertShaderCode = readFile(vertexShaderPath);
    auto fragShaderCode = readFile(fragmentShaderPath);
    
    shader->vertexShaderModule = createShaderModule(rendererContent, vertShaderCode); 
    shader->fragmentShaderModule = createShaderModule(rendererContent, fragShaderCode);
    
    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;
    vertShaderStageInfo.module = shader->vertexShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = shader->fragmentShaderModule;
    fragShaderStageInfo.pName = "main";

    // Geometry shader is optional. Checking if we should add it.
    //TODO: This feature is for now not supported since geometry shaders don't work on mac. Will be supported once a crossplatform solution is created
    if(geometryShaderPath != "") {

        auto geometryShaderCode = readFile(geometryShaderPath);
        shader->geometryShaderModule = createShaderModule(rendererContent, geometryShaderCode);

        VkPipelineShaderStageCreateInfo geometryShaderStageInfo{};
        geometryShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
        geometryShaderStageInfo.stage = VK_SHADER_STAGE_GEOMETRY_BIT;
        geometryShaderStageInfo.module = shader->geometryShaderModule;
        geometryShaderStageInfo.pName = "main";

        shader->shaderStages[2] = geometryShaderStageInfo;

        shader->shaderCount = 3; 
    }

    // Assigning vertex and fragment shaders
    shader->shaderStages[0] = vertShaderStageInfo;
    shader->shaderStages[1] = fragShaderStageInfo; 

    logger(SUCCESS, "Successfully created shader!"); 

    return *shader; 
}

void freeShader(RendererContent& rendererContent, Shader& shader) {
    vkDestroyShaderModule(rendererContent.device, shader.vertexShaderModule, nullptr);
    vkDestroyShaderModule(rendererContent.device, shader.fragmentShaderModule, nullptr);

    delete &shader;
}

//TODO: Move this to a read / write source file instead of this helper function? 
std::vector<char> readFile(const std::string& fileName) {

    std::ifstream file(fileName, std::ios::ate | std::ios::binary); 

    if (!file.is_open()) { 
        logger(ERROR, "Failed to open file!"); 
        throw std::runtime_error("failed to open file!"); 
    }

    size_t fileSize = (size_t) file.tellg(); 
    std::vector<char> buffer(fileSize);

    file.seekg(0); 
    file.read(buffer.data(), fileSize);

    file.close(); 

    return buffer;
};

VkShaderModule createShaderModule(RendererContent& rendererContent, const std::vector<char>& code) {
            
    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO; 
    createInfo.codeSize = code.size(); 
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;

    if (vkCreateShaderModule(rendererContent.device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        logger(ERROR, "Failed to create shader module");
        throw std::runtime_error("failed to create shader module!"); 
    }
    return shaderModule;
};