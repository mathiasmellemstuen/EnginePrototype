#include "swapChainSupport.h"
#include "physicalDevice.h"
#include "../utility/debug.h"
#include <vulkan/vulkan.h>
SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR& surface) {
    Debug::log(INFO, "Querying for swap chain support"); 
    SwapChainSupportDetails details; 

    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(device, surface,  &details.capabilities);
    uint32_t formatCount; 
    vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface, &formatCount,  nullptr); 

    if (formatCount != 0) { 
        details.formats.resize(formatCount); 
        vkGetPhysicalDeviceSurfaceFormatsKHR(device, surface,  &formatCount, details.formats.data()); 
    }

    uint32_t presentModeCount; 
    vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface,  &presentModeCount, nullptr); 

    if (presentModeCount != 0) { 
        details.presentModes.resize(presentModeCount); 
        vkGetPhysicalDeviceSurfacePresentModesKHR(device, surface, &presentModeCount, details.presentModes.data()); 
    }

    Debug::log(SUCCESS, "Swap chain support query done!"); 
    return details; 
};