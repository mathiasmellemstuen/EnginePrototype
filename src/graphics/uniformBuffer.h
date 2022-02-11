#ifndef ENGINEPROTOTYPE_UNIFORMBUFFER
#define ENGINEPROTOTYPE_UNIFORMBUFFER

#include <vector>
#include <vulkan/vulkan.h>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>

class Renderer;

class UniformBuffer {
    public:
        std::vector<VkBuffer> uniformBuffers;
        std::vector<VkDeviceMemory> uniformBuffersMemory;
        void update(uint32_t currentImage, glm::mat4& view, glm::mat4& projection, glm::mat4& model);
        void setPosition(glm::vec3 position);
        void create(); 
        UniformBuffer(Renderer& renderer); 
        void clean(); 
    private:
        size_t allocatedSwapChainSize;
        Renderer& renderer;
};

#endif
