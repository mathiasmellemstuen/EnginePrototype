#ifndef ENGINEPROTOTYPE_GRAPHICSPIPELINE
#define ENGINEPROTOTYPE_GRAPHICSPIPELINE

#include "swapChain.h"
#include "logicalDevice.h"
#include "shader.h"
#include "descriptorSetLayout.h"
#include "physicalDevice.h"

#include <vulkan/vulkan.h>

class GraphicsPipeline {
    public: 
        VkPipeline graphicsPipeline;
        VkPipelineLayout pipelineLayout;
        VkDevice* device; 
        VkRenderPass renderPass; 
        void create(LogicalDevice& logicalDevice, SwapChain& swapChain, Shader& shader, DescriptorSetLayout& descriptorSetLayout);
        GraphicsPipeline(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, SwapChain& swapChain, Shader& shader, DescriptorSetLayout& descriptorSetLayout); 
        ~GraphicsPipeline();
        void createRenderPass(PhysicalDevice& physicalDevice, SwapChain& swapChain); 
    private:
};

#endif
