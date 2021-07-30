#ifndef ENGINEPROTOTYPE_UNIFORMBUFFER
#define ENGINEPROTOTYPE_UNIFORMBUFFER

#include <vector>
#include <vulkan/vulkan.h>
#include "logicalDevice.h"
#include "swapChain.h"
#include "physicalDevice.h"

class UniformBuffer {
    public:
        VkDevice* device; 
        std::vector<VkBuffer> uniformBuffers;
        std::vector<VkDeviceMemory> uniformBuffersMemory;
        void update(uint32_t currentImage);
        void create(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, SwapChain& swapChain); 
        UniformBuffer(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, SwapChain& swapChain); 
        ~UniformBuffer(); 
    private:
        size_t allocatedSwapChainSize;
};

#endif