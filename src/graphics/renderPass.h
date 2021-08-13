#ifndef ENGINEPROTOTYPE_RENDERPASS
#define ENGINEPROTOTYPE_RENDERPASS

#include <vulkan/vulkan.h>

class Renderer;

class RenderPass {
    public:
        VkRenderPass renderPass; 
        RenderPass(Renderer& renderer);
        ~RenderPass();
        void create();
    private:
        Renderer& renderer;
};

#endif