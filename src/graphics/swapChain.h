#ifndef ENGINEPROTOTYPE_SWAPCHAIN
#define ENGINEPROTOTYPE_SWAPCHAIN
#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <vector>
#include "physicalDevice.h"
#include "logicalDevice.h"

class SwapChain {
    public:
        VkSwapchainKHR swapChain;
        std::vector<VkImage> swapChainImages;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;
        SwapChain(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice);
        ~SwapChain();
    private:
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR&  capabilities);
        VkDevice* device;
};

#endif