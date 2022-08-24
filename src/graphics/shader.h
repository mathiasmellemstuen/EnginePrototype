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
	VkShaderModule computeShaderModule; 
    VkPipelineShaderStageCreateInfo shaderStages[4];
    uint16_t shaderCount; 
};

std::vector<char> readFile(const std::string& fileName);
VkShaderModule createShaderModule(Renderer& renderer, const std::vector<char>& code);
const Shader& createShader(Renderer& renderer, std::string vertexShaderPath, std::string fragmentShaderPath, std::string geometryShaderPath = "", std::string computeShaderPath = "");
void freeShader(Renderer& renderer, Shader& shader);

#endif
