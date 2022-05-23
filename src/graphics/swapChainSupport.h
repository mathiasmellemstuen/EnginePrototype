#ifndef ENGINEPROTOTYPE_SWAPCHAINSUPPORT
#define ENGINEPROTOTYPE_SWAPCHAINSUPPORT

#include <vulkan/vulkan.h>
#include <vector>

struct SwapChainSupportDetails { 
    VkSurfaceCapabilitiesKHR capabilities; 
    std::vector<VkSurfaceFormatKHR> formats; 
    std::vector<VkPresentModeKHR> presentModes; 
};

SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR& surface);

#endif