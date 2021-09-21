#ifndef ENGINEPROTOTYPE_GRAPHICSPIPELINE
#define ENGINEPROTOTYPE_GRAPHICSPIPELINE

#include <vulkan/vulkan.h>

class RenderObject;

class GraphicsPipeline {
    public: 
        VkPipeline graphicsPipeline;
        VkPipelineLayout pipelineLayout;
        void create();
        GraphicsPipeline(RenderObject& renderObject); 
        ~GraphicsPipeline();
        void createRenderPass(); 
    private:
        RenderObject& renderObject;
};

#endif
