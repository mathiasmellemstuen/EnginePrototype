#ifndef ENGINEPROTOTYPE_GRAPHICSPIPELINE
#define ENGINEPROTOTYPE_GRAPHICSPIPELINE

#include <vulkan/vulkan.h>

class Renderer;

class GraphicsPipeline {
    public: 
        VkPipeline graphicsPipeline;
        VkPipelineLayout pipelineLayout;
        void create();
        GraphicsPipeline(Renderer& renderer); 
        ~GraphicsPipeline();
        void createRenderPass(); 
    private:
        Renderer& renderer;
};

#endif
