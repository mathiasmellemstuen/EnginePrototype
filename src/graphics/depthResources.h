#ifndef ENGINEPROTOTYPE_DEPTHRESOURCES
#define ENGINEPROTOTYPE_DEPTHRESOURCES

#include <vulkan/vulkan.h>
#include "physicalDevice.h"
#include "imageViews.h"
#include "texture.h"
#include "swapChain.h"
#include "logicalDevice.h"

class DepthResources {
    public:
        VkDevice* device; 
        VkImage depthImage;
        VkDeviceMemory depthImageMemory;
        VkImageView depthImageView;
        void create(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, ImageViews& imageViews, SwapChain& swapChain); 
        DepthResources(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, ImageViews& imageViews, SwapChain& swapChain); 
        ~DepthResources();
};

#endif