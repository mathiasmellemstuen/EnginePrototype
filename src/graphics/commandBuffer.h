#ifndef ENGINEPROTOTYPE_COMMANDBUFFER
#define ENGINEPROTOTYPE_COMMANDBUFFER

#include <vulkan/vulkan.h>
#include "logicalDevice.h"
#include "physicalDevice.h"
#include "frameBuffers.h"
#include "swapChain.h"
#include "graphicsPipeline.h"
#include "vertex.h"
#include "vertexBuffer.h"

#include <vector>

class CommandBuffers {
    public:
        VkCommandPool commandPool;
        std::vector<VkCommandBuffer> commandBuffers;
        CommandBuffers(LogicalDevice& logicalDevice, PhysicalDevice& physicalDevice, FrameBuffers& frameBuffers, SwapChain& swapChain, GraphicsPipeline& graphicsPipeline, VertexBuffer& vertexBuffer); 
        void create(LogicalDevice& logicalDevice, PhysicalDevice& physicalDevice, FrameBuffers& frameBuffers, SwapChain& swapChain, GraphicsPipeline& graphicsPipeline, VertexBuffer& vertexBuffer);
        ~CommandBuffers(); 
    private:
        VkDevice* device; 
};

#endif