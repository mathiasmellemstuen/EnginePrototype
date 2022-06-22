#ifndef ENGINEPROTOTYPE_RENDERER
#define ENGINEPROTOTYPE_RENDERER

#include <vulkan/vulkan.h>
#include <vector>
#include "QueueFamilyIndices.h"
#include "window.h"
#include "eventManager.h"
#include <stdexcept>

struct RendererContent {
    VkInstance instance;
    VkSurfaceKHR surface;
    VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
    VkPhysicalDeviceProperties physicalDeviceProperties; 
    VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
    VkDevice device;
    VkQueue graphicsQueue;
    VkQueue presentQueue;
    VkSwapchainKHR swapChain;
    std::vector<VkImage> swapChainImages;
    VkFormat swapChainImageFormat;
    VkExtent2D swapChainExtent;
    std::vector<VkImageView> swapChainImageViews;
    VkRenderPass renderPass;
    VkRenderPass uiRenderPass; 
    VkCommandPool commandPool; 
    VkImage colorImage;
    VkDeviceMemory colorImageMemory;
    VkImageView colorImageView;
    VkImage depthImage;
    VkDeviceMemory depthImageMemory;
    VkImageView depthImageView;
    std::vector<VkFramebuffer> swapChainFramebuffers;
    std::vector<VkCommandBuffer> commandBuffers;
    std::vector<VkSemaphore> imageAvailableSemaphores;
    std::vector<VkSemaphore> renderFinishedSemaphores;
    std::vector<VkFence> imagesInFlight;
    std::vector<VkFence> inFlightFences;
    size_t currentFrame = 0;
};

void createCommandBuffers(RendererContent& rendererContent, uint32_t currentImage);
uint32_t findMemoryType(RendererContent& rendererContent, uint32_t typeFilter, VkMemoryPropertyFlags properties);
void createImage(RendererContent& rendererContent, uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
VkFormat findSupportedFormat(RendererContent& rendererContent, const std::vector<VkFormat>&candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
VkFormat findDepthFormat(RendererContent& rendererContent);
VkImageView createImageView(RendererContent& rendererContent, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
VkSampleCountFlagBits getMaxUsableSampleCount(RendererContent& rendererContent);
bool checkDeviceExtensionSupport(RendererContent& rendererContent);
QueueFamilyIndices findQueueFamilies(RendererContent& rendererContent);
bool isDeviceSuitable(RendererContent& rendererContent, const VkPhysicalDevice& device);
VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, Window& window);
void loop(RendererContent& rendererContent, Window& window, EventManager& eventManager);
void drawFrame(RendererContent& rendererContent, Window& window);
void cleanupSwapChain(RendererContent& rendererContent);
void reCreateSwapChain(RendererContent& rendererContent, Window& window);
RendererContent createRenderer(Window& window);
void destroyRenderer(RendererContent& rendererContent);
void allocateSwapchainDependentRendererContent(RendererContent& rendererContent, Window& window);
void createBuffer(RendererContent& rendererContent, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
VkCommandBuffer beginSingleTimeCommands(RendererContent& rendererContent);
void endSingleTimeCommands(RendererContent& rendererContent, VkCommandBuffer commandBuffer);
bool hasStencilComponent(VkFormat format);
void generateMipmaps(RendererContent& rendererContent, VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
void copyBuffer(RendererContent& rendererContent, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
#endif