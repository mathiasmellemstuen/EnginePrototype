#ifndef ENGINEPROTOTYPE_FRAMEBUFFERS
#define ENGINEPROTOTYPE_FRAMEBUFFERS

#include <vulkan/vulkan.h>
#include <vector>
#include "imageViews.h"
#include "logicalDevice.h"
#include "swapChain.h"
#include "graphicsPipeline.h"

class FrameBuffers {
    public:
        std::vector<VkFramebuffer> swapChainFramebuffers;
        FrameBuffers(LogicalDevice& logicalDevice, ImageViews& imageViews, SwapChain& swapChain, GraphicsPipeline& graphicsPipeline); 
        ~FrameBuffers(); 
    private: 
        VkDevice* device; 
};
#endif