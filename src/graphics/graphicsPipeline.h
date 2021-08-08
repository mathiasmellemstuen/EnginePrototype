#ifndef ENGINEPROTOTYPE_GRAPHICSPIPELINE
#define ENGINEPROTOTYPE_GRAPHICSPIPELINE

#include "swapChain.h"
#include "logicalDevice.h"
#include "shader.h"
#include "descriptorSetLayout.h"
#include "physicalDevice.h"

#include <vulkan/vulkan.h>

class Renderer;

class GraphicsPipeline {
    public: 
        VkPipeline graphicsPipeline;
        VkPipelineLayout pipelineLayout;
        VkRenderPass renderPass; 
        void create();
        GraphicsPipeline(Renderer& renderer); 
        ~GraphicsPipeline();
        void createRenderPass(); 
    private:
        Renderer& renderer;
};

#endif
