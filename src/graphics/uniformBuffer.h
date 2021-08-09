#ifndef ENGINEPROTOTYPE_UNIFORMBUFFER
#define ENGINEPROTOTYPE_UNIFORMBUFFER

#include <vector>
#include <vulkan/vulkan.h>

class Renderer;

class UniformBuffer {
    public:
        std::vector<VkBuffer> uniformBuffers;
        std::vector<VkDeviceMemory> uniformBuffersMemory;
        void update(uint32_t currentImage);
        void create(); 
        UniformBuffer(Renderer& renderer); 
        ~UniformBuffer(); 
    private:
        size_t allocatedSwapChainSize;
        Renderer& renderer;
};

#endif