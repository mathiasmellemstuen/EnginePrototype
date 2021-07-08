#ifndef ENGINEPROTOTYPE_GRAPHICSPIPELINE
#define ENGINEPROTOTYPE_GRAPHICSPIPELINE

#include "swapChain.h"
#include "logicalDevice.h"

#include <vulkan/vulkan.h>

class GraphicsPipeline {
    public: 
        VkPipelineLayout pipelineLayout;
        VkDevice* device; 
        GraphicsPipeline(LogicalDevice& logicalDevice, SwapChain& swapChain); 
        ~GraphicsPipeline();
};

#endif
