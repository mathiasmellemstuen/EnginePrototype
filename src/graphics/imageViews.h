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
        void create(SwapChain& swapChain, LogicalDevice& logicalDevice); 
        VkImageView createImageView(LogicalDevice& logicalDevice, VkImage image, VkFormat format);
    private:
        VkDevice* device; 
};

#endif