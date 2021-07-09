#include "swapChain.h"

#include <vulkan/vulkan.h>
#include <cstdint>
#include "physicalDevice.h"
#include "swapChainSupport.h"
#include "logicalDevice.h"
#include <stdexcept>
#include "../utility/properties.h"
#include "../utility/log.h"
#include "window.h"

#include <SDL2/SDL.h>


SwapChain::SwapChain(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, Window& window) {
    
    device = &logicalDevice.device;
    create(physicalDevice, logicalDevice, window); 
};

SwapChain::~SwapChain() {
    log(INFO, "Destroying swap chain"); 
    vkDestroySwapchainKHR(*device, swapChain, nullptr);
    log(SUCCESS, "Swap chain destroyed"); 
};

void SwapChain::create(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, Window& window) {
    log(INFO, "Setting up swap chain"); 

    SwapChainSupportDetails swapChainSupport =  querySwapChainSupport(physicalDevice.physicalDevice, *physicalDevice.surface); 

    VkSurfaceFormatKHR surfaceFormat = chooseSwapSurfaceFormat(swapChainSupport.formats);
    VkPresentModeKHR presentMode =  chooseSwapPresentMode(swapChainSupport.presentModes); 
    VkExtent2D extent =  chooseSwapExtent(swapChainSupport.capabilities, window);

    uint32_t imageCount = swapChainSupport.capabilities.minImageCount + 1;

    if (swapChainSupport.capabilities.maxImageCount > 0 && imageCount >  swapChainSupport.capabilities.maxImageCount) { 
        imageCount = swapChainSupport.capabilities.maxImageCount;
    }

    VkSwapchainCreateInfoKHR createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR;
    createInfo.surface = *physicalDevice.surface;

    createInfo.minImageCount = imageCount;
    createInfo.imageFormat = surfaceFormat.format;
    createInfo.imageColorSpace = surfaceFormat.colorSpace;
    createInfo.imageExtent = extent;
    createInfo.imageArrayLayers = 1;
    createInfo.imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT;

    QueueFamilyIndices indices = physicalDevice.findQueueFamilies(physicalDevice.physicalDevice);
    uint32_t queueFamilyIndices[] = {indices.graphicsFamily.value(), indices.presentFamily.value()}; 

    if (indices.graphicsFamily != indices.presentFamily) {
        createInfo.imageSharingMode = VK_SHARING_MODE_CONCURRENT;
        createInfo.queueFamilyIndexCount = 2;
        createInfo.pQueueFamilyIndices = queueFamilyIndices;
    } else { 
        createInfo.imageSharingMode = VK_SHARING_MODE_EXCLUSIVE;
        createInfo.queueFamilyIndexCount = 0; // Optional
        createInfo.pQueueFamilyIndices = nullptr; // Optional 
    }

    createInfo.preTransform =  swapChainSupport.capabilities.currentTransform;
    createInfo.compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR;
    createInfo.presentMode = presentMode;
    createInfo.clipped = VK_TRUE;
    createInfo.oldSwapchain = VK_NULL_HANDLE;

    if (vkCreateSwapchainKHR(*device, &createInfo, nullptr, &swapChain)  != VK_SUCCESS) { 
        log(ERROR, "Failed to create swap chain!"); 
        throw std::runtime_error("failed to create swap chain!");
    }

    vkGetSwapchainImagesKHR(*device, swapChain, &imageCount, nullptr);
    swapChainImages.resize(imageCount);
    vkGetSwapchainImagesKHR(*device, swapChain, &imageCount,
    swapChainImages.data());

    swapChainImageFormat = surfaceFormat.format;
    swapChainExtent = extent;

    log(SUCCESS, "Created swap chain"); 
};

VkSurfaceFormatKHR SwapChain::chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats) {

    for (const auto& availableFormat : availableFormats) { 
        if (availableFormat.format == VK_FORMAT_B8G8R8A8_SRGB &&  availableFormat.colorSpace == VK_COLOR_SPACE_SRGB_NONLINEAR_KHR) { 
            return availableFormat; 
        } 
    }

    return availableFormats[0]; 
};

VkPresentModeKHR SwapChain::chooseSwapPresentMode(const  std::vector<VkPresentModeKHR>& availablePresentModes) { 
    
    for (const auto& availablePresentMode : availablePresentModes) {
        if (availablePresentMode == VK_PRESENT_MODE_MAILBOX_KHR) {
            return availablePresentMode; 
        }
    } 

    return VK_PRESENT_MODE_FIFO_KHR;
};

VkExtent2D SwapChain::chooseSwapExtent(const VkSurfaceCapabilitiesKHR&  capabilities, Window& window) { 

    if (capabilities.currentExtent.width != UINT32_MAX) {
        return capabilities.currentExtent;
    }

    int width, height;
    SDL_GetWindowSize(window.sdlWindow, &width, &height);
    VkExtent2D actualExtent = {static_cast<uint32_t>(width),static_cast<uint32_t>(height)};

    actualExtent.width = std::max(capabilities.minImageExtent.width, std::min(capabilities.maxImageExtent.width, actualExtent.width)); 
    actualExtent.height = std::max(capabilities.minImageExtent.height, std::min(capabilities.maxImageExtent.height, actualExtent.height)); 

    return actualExtent;
};
