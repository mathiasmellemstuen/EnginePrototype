#ifndef ENGINEPROTOTYPE_SHADER
#define ENGINEPROTOTYPE_SHADER

#include <vector>
#include <string>

#include <vulkan/vulkan.h>

class Renderer;

class Shader {
    public:
        Shader(Renderer& renderer, std::string vertexShaderPath, std::string fragmentShaderPath); 
        void clean();
        VkShaderModule vertexShaderModule; 
        VkShaderModule fragmentShaderModule;
        VkPipelineShaderStageCreateInfo shaderStages[2]; 
    private:
        std::vector<char> readFile(const std::string& fileName);
        VkShaderModule createShaderModule(const std::vector<char>& code);
        Renderer& renderer;
};  

#endif