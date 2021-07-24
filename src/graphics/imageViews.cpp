#include "imageViews.h"

#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>
#include "logicalDevice.h"
#include "../utility/debug.h"

ImageViews::ImageViews(SwapChain& swapChain, LogicalDevice& logicalDevice) {
    this->device = &logicalDevice.device;
    swapChainImageViews.resize(swapChain.swapChainImages.size());
    
    create(swapChain, logicalDevice); 
};

void ImageViews::create(SwapChain& swapChain, LogicalDevice& logicalDevice) {
    Debug::log(INFO, "Create image views"); 

    for (size_t i = 0; i < swapChain.swapChainImages.size(); i++) {
        
        VkImageViewCreateInfo createInfo{};
        createInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
        createInfo.image = swapChain.swapChainImages[i];
        createInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
        createInfo.format = swapChain.swapChainImageFormat;
        createInfo.components.r = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.g = VK_COMPONENT_SWIZZLE_IDENTITY; 
        createInfo.components.b = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.components.a = VK_COMPONENT_SWIZZLE_IDENTITY;
        createInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
        createInfo.subresourceRange.baseMipLevel = 0;
        createInfo.subresourceRange.levelCount = 1;
        createInfo.subresourceRange.baseArrayLayer = 0;
        createInfo.subresourceRange.layerCount = 1;
        
        if (vkCreateImageView(*this->device, &createInfo, nullptr,  &swapChainImageViews[i]) != VK_SUCCESS) { 
            throw std::runtime_error("failed to create image views!");
        }
    }

    Debug::log(SUCCESS, "Image views created!"); 
};

ImageViews::~ImageViews(){
    Debug::log(INFO, "Destroying image views"); 
    for (auto imageView : swapChainImageViews) {
        vkDestroyImageView(*this->device, imageView, nullptr);
    }

    Debug::log(SUCCESS, "Image views destroyed!");
};