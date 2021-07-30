#include "imageViews.h"

#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>
#include "logicalDevice.h"
#include "../utility/debug.h"

ImageViews::ImageViews(SwapChain& swapChain, LogicalDevice& logicalDevice) {
    this->device = &logicalDevice.device;
    create(swapChain, logicalDevice); 
};

void ImageViews::create(SwapChain& swapChain, LogicalDevice& logicalDevice) {
    Debug::log(INFO, "Create image views");

    swapChainImageViews.resize(swapChain.swapChainImages.size());
    for (uint32_t i = 0; i < swapChain.swapChainImages.size(); i++) {
        swapChainImageViews[i] = createImageView(logicalDevice, swapChain.swapChainImages[i], swapChain.swapChainImageFormat);
    }

    Debug::log(SUCCESS, "Image views created!"); 
};

VkImageView ImageViews::createImageView(LogicalDevice& logicalDevice, VkImage image, VkFormat format) {

    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = 1;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VkImageView imageView;

    if (vkCreateImageView(logicalDevice.device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture image view!");
    }

    return imageView;
}

ImageViews::~ImageViews(){
    Debug::log(INFO, "Destroying image views"); 
    for (auto imageView : swapChainImageViews) {
        vkDestroyImageView(*this->device, imageView, nullptr);
    }

    Debug::log(SUCCESS, "Image views destroyed!");
};