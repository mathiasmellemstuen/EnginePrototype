#ifndef ENGINEPROTOTYPE_RENDERPASS
#define ENGINEPROTOTYPE_RENDERPASS

#include <vulkan/vulkan.h>

class Renderer;

class RenderPass {
    public:
        VkRenderPass renderPass; 
        RenderPass(Renderer& renderer);
        void clean();
        void create();
    private:
        Renderer& renderer;
};

#endif