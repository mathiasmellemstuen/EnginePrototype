#ifndef ENGINEPROTOTYPE_SWAPCHAIN
#define ENGINEPROTOTYPE_SWAPCHAIN

#include <vulkan/vulkan.h>
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
        void create(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice);
        // void reCreate(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, ImageViews& imageViews, GraphicsPipeline& graphicsPipeline, Shader& shader, FrameBuffers& frameBuffers, CommandBuffers& commandBuffers);
    private:
        VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
        VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
        VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR&  capabilities);
        VkDevice* device;
        void cleanUp(); 
};

#endif