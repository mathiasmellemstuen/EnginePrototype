#include "colorResources.h"
#include "physicalDevice.h"
#include "swapChain.h"
#include "imageViews.h"
#include "logicalDevice.h"
#include "renderer.h"

void ColorResources::create() {
    
    VkFormat colorFormat = renderer.swapChain.swapChainImageFormat;
    renderer.createImage(renderer.swapChain.swapChainExtent.width, renderer.swapChain.swapChainExtent.height, 1, renderer.physicalDevice.msaaSamples, colorFormat, VK_IMAGE_TILING_OPTIMAL, VK_IMAGE_USAGE_TRANSIENT_ATTACHMENT_BIT | VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, colorImage, colorImageMemory);
    colorImageView = renderer.imageViews.createImageView(colorImage, colorFormat, VK_IMAGE_ASPECT_COLOR_BIT, 1);
};

ColorResources::ColorResources(Renderer& renderer) : renderer(renderer) {

    create(); 
};

ColorResources::~ColorResources() {
    vkDestroyImageView(renderer.logicalDevice.device, colorImageView, nullptr);
    vkDestroyImage(renderer.logicalDevice.device, colorImage, nullptr);
    vkFreeMemory(renderer.logicalDevice.device, colorImageMemory, nullptr);

};