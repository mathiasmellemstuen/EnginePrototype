#ifndef ENGINEPROTOTYPE_FRAMEBUFFERS
#define ENGINEPROTOTYPE_FRAMEBUFFERS

#include <vulkan/vulkan.h>
#include <vector>
#include "imageViews.h"
#include "logicalDevice.h"
#include "swapChain.h"
#include "graphicsPipeline.h"
#include "depthResources.h" 
#include "colorResources.h"

class Renderer;

class FrameBuffers {
    public:
        std::vector<VkFramebuffer> swapChainFramebuffers;
        FrameBuffers(Renderer& renderer);
        void create(); 
        ~FrameBuffers(); 
    private: 
        Renderer& renderer;
    };
#endif