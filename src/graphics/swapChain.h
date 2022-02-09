#ifndef ENGINEPROTOTYPE_SWAPCHAIN
#define ENGINEPROTOTYPE_SWAPCHAIN

#include <vulkan/vulkan.h>
#include <vector>
#include "window.h"

class Renderer;

class SwapChain {
    public:
        VkSwapchainKHR swapChain;
        std::vector<VkImage> swapChainImages;
        VkFormat swapChainImageFormat;
        VkExtent2D swapChainExtent;
        SwapChain(Renderer& renderer);
        void clean();
        void create();
    private:
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR&  capabilities, Window& window);
        Renderer& renderer;
};

#endif