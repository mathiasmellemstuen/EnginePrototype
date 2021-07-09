#ifndef ENGINEPROTOTYPE_COMMANDBUFFER
#define ENGINEPROTOTYPE_COMMANDBUFFER

#include <vulkan/vulkan.h>
#include "logicalDevice.h"
#include "physicalDevice.h"
#include "frameBuffers.h"
#include "swapChain.h"
#include "graphicsPipeline.h"

#include <vector>

class CommandBuffers {
    public:
        VkCommandPool commandPool;
        std::vector<VkCommandBuffer> commandBuffers;
        CommandBuffers(LogicalDevice& logicalDevice, PhysicalDevice& physicalDevice, FrameBuffers& frameBuffers, SwapChain& swapChain, GraphicsPipeline& graphicsPipeline); 
        ~CommandBuffers(); 
    private:
        VkDevice* device; 
};

#endif