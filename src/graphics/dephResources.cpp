#include "depthResources.h"
#include "vulkanHelperFunctions.h"
#include "../utility/debug.h"

void DepthResources::create(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, ImageViews& imageViews, SwapChain& swapChain) {

    Debug::log(INFO, "Creating depth resources.");

    VkFormat depthFormat = findDepthFormat(physicalDevice);

    createImage(physicalDevice, logicalDevice, swapChain.swapChainExtent.width, swapChain.swapChainExtent.height, 1, VK_SAMPLE_COUNT_1_BIT, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);    depthImageView = imageViews.createImageView(logicalDevice, depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);

    Debug::log(SUCCESS, "Created depth resources!"); 
}

DepthResources::DepthResources(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, ImageViews& imageViews, SwapChain& swapChain) {
    device = &logicalDevice.device; 
    create(physicalDevice, logicalDevice, imageViews, swapChain);
}

DepthResources::~DepthResources() {
    vkDestroyImageView(*device, depthImageView, nullptr);
    vkDestroyImage(*device, depthImage, nullptr);
    vkFreeMemory(*device, depthImageMemory, nullptr);
}