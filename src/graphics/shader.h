#ifndef ENGINEPROTOTYPE_SHADER
#define ENGINEPROTOTYPE_SHADER

#include <vector>
#include <string>

#include <vulkan/vulkan.h>
#include "renderer.h"

struct Shader {
    VkShaderModule vertexShaderModule; 
    VkShaderModule fragmentShaderModule;
    VkShaderModule geometryShaderModule;
    VkPipelineShaderStageCreateInfo shaderStages[3];
    uint16_t shaderCount; 
};

std::vector<char> readFile(const std::string& fileName);
VkShaderModule createShaderModule(RendererContent& rendererContent, const std::vector<char>& code);
Shader createShader(RendererContent& rendererContent, std::string vertexShaderPath, std::string fragmentShaderPath, std::string geometryShaderPath = "");
void freeShader(RendererContent& rendererContent, Shader& shader);

#endif