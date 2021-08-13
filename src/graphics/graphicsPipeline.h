#ifndef ENGINEPROTOTYPE_GRAPHICSPIPELINE
#define ENGINEPROTOTYPE_GRAPHICSPIPELINE

#include <vulkan/vulkan.h>

class RendererInfo;

class GraphicsPipeline {
    public: 
        VkPipeline graphicsPipeline;
        VkPipelineLayout pipelineLayout;
        void create();
        GraphicsPipeline(RendererInfo& rendererInfo); 
        ~GraphicsPipeline();
        void createRenderPass(); 
    private:
        RendererInfo& rendererInfo;
};

#endif
