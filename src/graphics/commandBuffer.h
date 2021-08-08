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
#include "descriptorSetLayout.h"
#include "descriptorPool.h"

#include <vector>

class Renderer;

class CommandBuffers {
    public:
        std::vector<VkCommandBuffer> commandBuffers;
        CommandBuffers(Renderer& renderer); 
        void create();
        ~CommandBuffers(); 
    private:
        Renderer& renderer;
};

#endif