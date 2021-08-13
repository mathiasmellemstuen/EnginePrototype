#include "depthResources.h"
#include "../utility/debug.h"

#include "renderer.h"

void DepthResources::create() {

    Debug::log(INFO, "Creating depth resources.");

    VkFormat depthFormat = renderer.findDepthFormat();

    renderer.createImage(renderer.swapChain.swapChainExtent.width, renderer.swapChain.swapChainExtent.height, 1, renderer.physicalDevice.msaaSamples, depthFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, depthImage, depthImageMemory);
    
    depthImageView = renderer.imageViews.createImageView(depthImage, depthFormat, VK_IMAGE_ASPECT_DEPTH_BIT, 1);

    Debug::log(SUCCESS, "Created depth resources!"); 
}

DepthResources::DepthResources(Renderer& renderer) : renderer(renderer) {
    create();
}

DepthResources::~DepthResources() {
    vkDestroyImageView(renderer.logicalDevice.device, depthImageView, nullptr);
    vkDestroyImage(renderer.logicalDevice.device, depthImage, nullptr);
    vkFreeMemory(renderer.logicalDevice.device, depthImageMemory, nullptr);
}