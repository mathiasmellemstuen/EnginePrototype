#ifndef ENGINEPROTOTYPE_SWAPCHAINSUPPORT
#define ENGINEPROTOTYPE_SWAPCHAINSUPPORT

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>

struct SwapChainSupportDetails { 
    VkSurfaceCapabilitiesKHR capabilities; 
    std::vector<VkSurfaceFormatKHR> formats; 
    std::vector<VkPresentModeKHR> presentModes; 
};

SwapChainSupportDetails querySwapChainSupport(VkPhysicalDevice device, VkSurfaceKHR& surface);

#endif