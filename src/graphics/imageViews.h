#ifndef ENGINEPROTOTYPE_IMAGEVIEWS
#define ENGINEPROTOTYPE_IMAGEVIEWS

#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>
#include "swapChain.h"
#include "logicalDevice.h"

class ImageViews {
    public: 
        std::vector<VkImageView> swapChainImageViews;
        ImageViews(SwapChain& swapChain, LogicalDevice& logicalDevice); 
        ~ImageViews(); 
    private:
        VkDevice* device; 
};

#endif