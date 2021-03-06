#ifndef ENGINEPROTOTYPE_RENDERER
#define ENGINEPROTOTYPE_RENDERER

#include <vulkan/vulkan.h>
#include <vector>
#include "QueueFamilyIndices.h"
#include "window.h"
#include "eventManager.h"
#include <stdexcept>

struct Renderer {
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

void createCommandBuffers(Renderer& renderer, uint32_t currentImage);
uint32_t findMemoryType(Renderer& renderer, uint32_t typeFilter, VkMemoryPropertyFlags properties);
void createImage(Renderer& renderer, uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
VkFormat findSupportedFormat(Renderer& renderer, const std::vector<VkFormat>&candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
VkFormat findDepthFormat(Renderer& renderer);
VkImageView createImageView(Renderer& renderer, VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
VkSampleCountFlagBits getMaxUsableSampleCount(Renderer& renderer);
bool checkDeviceExtensionSupport(Renderer& renderer);
QueueFamilyIndices findQueueFamilies(Renderer& renderer);
bool isDeviceSuitable(Renderer& renderer, const VkPhysicalDevice& device);
VkSurfaceFormatKHR chooseSwapSurfaceFormat(const std::vector<VkSurfaceFormatKHR>& availableFormats);
VkPresentModeKHR chooseSwapPresentMode(const std::vector<VkPresentModeKHR>& availablePresentModes);
VkExtent2D chooseSwapExtent(const VkSurfaceCapabilitiesKHR& capabilities, Window& window);
void loop(Renderer& renderer, Window& window, EventManager& eventManager);
void drawFrame(Renderer& renderer, Window& window);
void cleanupSwapChain(Renderer& renderer);
void reCreateSwapChain(Renderer& renderer, Window& window);
const Renderer& createRenderer(Window& window);
void destroyRenderer(Renderer& renderer);
void allocateSwapchainDependentRendererContent(Renderer& renderer, Window& window);
void createBuffer(Renderer& renderer, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
VkCommandBuffer beginSingleTimeCommands(Renderer& renderer);
void endSingleTimeCommands(Renderer& renderer, VkCommandBuffer commandBuffer);
bool hasStencilComponent(VkFormat format);
void generateMipmaps(Renderer& renderer, VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
void copyBuffer(Renderer& renderer, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
#endif