#include "shader.h"

#include <vulkan/vulkan.h>
#include <fstream>
#include <vector>
#include <string>
#include "../utility/log.h"

Shader::Shader(LogicalDevice& logicalDevice, std::string vertexShaderPath, std::string fragmentShaderPath) {
    
    log(INFO, "Creating shader"); 

    this->device = &logicalDevice.device; 

    auto vertShaderCode = readFile(vertexShaderPath);
    auto fragShaderCode = readFile(fragmentShaderPath);

    this->vertexShaderModule = createShaderModule(vertShaderCode); 
    this->fragmentShaderModule = createShaderModule(fragShaderCode);

    VkPipelineShaderStageCreateInfo vertShaderStageInfo{};
    vertShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO;
    vertShaderStageInfo.stage = VK_SHADER_STAGE_VERTEX_BIT;

    vertShaderStageInfo.module = this->vertexShaderModule;
    vertShaderStageInfo.pName = "main";

    VkPipelineShaderStageCreateInfo fragShaderStageInfo{};
    fragShaderStageInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO; 
    fragShaderStageInfo.stage = VK_SHADER_STAGE_FRAGMENT_BIT;
    fragShaderStageInfo.module = this->fragmentShaderModule;
    fragShaderStageInfo.pName = "main";

    this->shaderStages[0] = vertShaderStageInfo;
    this->shaderStages[1] = fragShaderStageInfo; 

    log(SUCCESS, "Successfully created shader!"); 
};

Shader::~Shader() {

    vkDestroyShaderModule(*this->device, this->vertexShaderModule, nullptr);
    vkDestroyShaderModule(*this->device, this->fragmentShaderModule, nullptr);
};

std::vector<char> Shader::readFile(const std::string& fileName) {

    std::ifstream file(fileName, std::ios::ate | std::ios::binary); 

    if (!file.is_open()) { 
        log(ERROR, "Failed to open file!"); 
        throw std::runtime_error("failed to open file!"); 
    }

    size_t fileSize = (size_t) file.tellg(); 
    std::vector<char> buffer(fileSize);

    file.seekg(0); 
    file.read(buffer.data(), fileSize);

    file.close(); 

    return buffer;
};

VkShaderModule Shader::createShaderModule(const std::vector<char>& code) {

    VkShaderModuleCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO; 
    createInfo.codeSize = code.size(); 
    createInfo.pCode = reinterpret_cast<const uint32_t*>(code.data());

    VkShaderModule shaderModule;

    if (vkCreateShaderModule(*this->device, &createInfo, nullptr, &shaderModule) != VK_SUCCESS) {
        log(ERROR, "Failed to create shader module");
        throw std::runtime_error("failed to create shader module!"); 
    }
    return shaderModule;
};