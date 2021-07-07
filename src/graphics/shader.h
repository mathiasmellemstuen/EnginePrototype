#ifndef ENGINEPROTOTYPE_SHADER
#define ENGINEPROTOTYPE_SHADER

#include <vector>
#include <string>

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

#include "logicalDevice.h"

class Shader {
    public:
        Shader(LogicalDevice& logicalDevice, std::string vertexShaderPath, std::string fragmentShaderPath); 
        ~Shader();
        VkShaderModule vertexShaderModule; 
        VkShaderModule fragmentShaderModule;
        VkPipelineShaderStageCreateInfo shaderStages[2]; 
    private:
        std::vector<char> readFile(const std::string& fileName);
        VkShaderModule createShaderModule(const std::vector<char>& code);
        VkDevice* device; 
};  

#endif