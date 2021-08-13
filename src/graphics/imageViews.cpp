#include "imageViews.h"

#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>
#include "logicalDevice.h"
#include "../utility/debug.h"

#include "renderer.h"

ImageViews::ImageViews(Renderer& renderer) : renderer(renderer) {
    create(); 
};

void ImageViews::create() {
    Debug::log(INFO, "Create image views");

    swapChainImageViews.resize(renderer.swapChain.swapChainImages.size());

    for (uint32_t i = 0; i < renderer.swapChain.swapChainImages.size(); i++) {
        swapChainImageViews[i] = createImageView(renderer.swapChain.swapChainImages[i], renderer.swapChain.swapChainImageFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
    }

    Debug::log(SUCCESS, "Image views created!"); 
};

VkImageView ImageViews::createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels) {

    VkImageViewCreateInfo viewInfo{};
    viewInfo.sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO;
    viewInfo.image = image;
    viewInfo.viewType = VK_IMAGE_VIEW_TYPE_2D;
    viewInfo.format = format;
    viewInfo.subresourceRange.aspectMask = aspectFlags;
    viewInfo.subresourceRange.baseMipLevel = 0;
    viewInfo.subresourceRange.levelCount = mipLevels;
    viewInfo.subresourceRange.baseArrayLayer = 0;
    viewInfo.subresourceRange.layerCount = 1;

    VkImageView imageView;

    if (vkCreateImageView(renderer.logicalDevice.device, &viewInfo, nullptr, &imageView) != VK_SUCCESS) {
        throw std::runtime_error("failed to create texture image view!");
    }

    return imageView;
}

ImageViews::~ImageViews(){
    Debug::log(INFO, "Destroying image views"); 
    for (auto imageView : swapChainImageViews) {
        vkDestroyImageView(renderer.logicalDevice.device, imageView, nullptr);
    }

    Debug::log(SUCCESS, "Image views destroyed!");
};