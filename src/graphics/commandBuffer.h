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
#include "commandPool.h"

#include <vector>

class CommandBuffers {
    public:
        std::vector<VkCommandBuffer> commandBuffers;
        CommandBuffers(LogicalDevice& logicalDevice, PhysicalDevice& physicalDevice, FrameBuffers& frameBuffers, SwapChain& swapChain, GraphicsPipeline& graphicsPipeline, VertexBuffer& vertexBuffer, CommandPool& commandPool); 
        void create(LogicalDevice& logicalDevice, PhysicalDevice& physicalDevice, FrameBuffers& frameBuffers, SwapChain& swapChain, GraphicsPipeline& graphicsPipeline, VertexBuffer& vertexBuffer, CommandPool& commandPool);
        static void copyBuffer(LogicalDevice& logicalDevice, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        ~CommandBuffers(); 
    private:
        VkDevice* device; 
};

#endif