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
        SwapChain(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, const uint32_t windowWidth, const uint32_t windowHeight);
        ~SwapChain();
    private:
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR&  capabilities, const uint32_t width, const uint32_t height);
        VkDevice* device;
};

#endif