#ifndef ENGINEPROTOTYPE_GRAPHICSPIPELINE
#define ENGINEPROTOTYPE_GRAPHICSPIPELINE

#include "swapChain.h"
#include "logicalDevice.h"
#include "shader.h"

#include <vulkan/vulkan.h>

class GraphicsPipeline {
    public: 
        VkPipeline graphicsPipeline;
        VkPipelineLayout pipelineLayout;
        VkDevice* device; 
        VkRenderPass renderPass; 
        GraphicsPipeline(LogicalDevice& logicalDevice, SwapChain& swapChain, Shader& shader); 
        void create(LogicalDevice& logicalDevice, SwapChain& swapChain, Shader& shader);
        ~GraphicsPipeline();
        void createRenderPass(SwapChain& swapChain); 
    private:
};

#endif
