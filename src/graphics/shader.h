#ifndef ENGINEPROTOTYPE_SHADER
#define ENGINEPROTOTYPE_SHADER

#include <vector>
#include <string>

#include <vulkan/vulkan.h>
#include "renderer.h"

struct Shader {
    VkShaderModule vertexShaderModule; 
    VkShaderModule fragmentShaderModule;
    VkPipelineShaderStageCreateInfo shaderStages[2]; 
};

std::vector<char> readFile(const std::string& fileName);
VkShaderModule createShaderModule(RendererContent& rendererContent, const std::vector<char>& code);
Shader createShader(RendererContent& rendererContent, std::string vertexShaderPath, std::string fragmentShaderPath);
void freeShader(RendererContent& rendererContent, Shader& shader);

#endif