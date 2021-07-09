#ifndef ENGINEPROTOTYPE_SWAPCHAIN
#define ENGINEPROTOTYPE_SWAPCHAIN

#include <vulkan/vulkan.h>
#include <vector>
#include "physicalDevice.h"
#include "logicalDevice.h"
#include "window.h"

class SwapChain {
    public:
        VkSwapchainKHR swapChain;
        std::vector<VkImage> swapChainImages;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;
        SwapChain(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, Window& window);
        ~SwapChain();
        void create(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, Window& window);
    private:
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR&  capabilities, Window& window);
        VkDevice* device;
};

#endif