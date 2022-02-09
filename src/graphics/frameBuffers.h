#ifndef ENGINEPROTOTYPE_FRAMEBUFFERS
#define ENGINEPROTOTYPE_FRAMEBUFFERS

#include <vulkan/vulkan.h>
#include <vector>

class Renderer;

class FrameBuffers {
    public:
        std::vector<VkFramebuffer> swapChainFramebuffers;
        FrameBuffers(Renderer& renderer);
        void create(); 
        void clean(); 
    private: 
        Renderer& renderer;
    };
#endif