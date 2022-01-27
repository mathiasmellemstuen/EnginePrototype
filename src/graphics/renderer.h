#ifndef ENGINEPROTOTYPE_RENDERER
#define ENGINEPROTOTYPE_RENDERER

#include "vulkanInstance.h"
#include "window.h"
#include "physicalDevice.h"
#include "logicalDevice.h"
#include "swapChain.h"
#include "imageViews.h"
#include "shader.h"
#include "descriptorSetLayout.h"
#include "renderPass.h"
#include "graphicsPipeline.h"
#include "frameBuffers.h"
#include "commandPool.h"
#include "vertexBuffer.h"
#include "uniformBuffer.h"
#include "descriptorPool.h"
#include "commandBuffer.h"
#include "syncObjects.h"
#include "texture.h"
#include "depthResources.h"
#include "colorResources.h"
#include "renderObject.h"
#include "eventManager.h"

#include <functional>
#include <vector>

class Renderer {
    public:
        Window& window;
        EventManager eventManager;
        VulkanInstance vulkanInstance;
        PhysicalDevice physicalDevice; 
        LogicalDevice logicalDevice;
        SwapChain swapChain;
        ImageViews imageViews;
        RenderPass renderPass;
        CommandPool commandPool;
        ColorResources colorResources;
        DepthResources depthResources;
        FrameBuffers frameBuffers;
        UniformBuffer uniformBuffer; 
        CommandBuffers commandBuffers;
        SyncObjects syncObjects;
        RenderObject* currentRenderObject; 

        uint64_t now;
        uint64_t last;

        // std::function<void(VkCommandBuffer& commandBuffer, int currentCommandBuffer, uint32_t currentImage)> renderFunction;
        // std::function<void(float deltaTime)> updateFunction;

        Renderer(Window& window);
        void loop();
        void createBuffer(VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
        uint32_t findMemoryType(uint32_t typeFilter, VkMemoryPropertyFlags properties);
        VkCommandBuffer beginSingleTimeCommands();
        void endSingleTimeCommands(VkCommandBuffer commandBuffer);
        VkFormat findSupportedFormat(const std::vector<VkFormat>&candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
        VkFormat findDepthFormat();
        bool hasStencilComponent(VkFormat format);
        void createImage(uint32_t width, uint32_t height, uint32_t mipLevels, VkSampleCountFlagBits numSamples, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
        void generateMipmaps(VkImage image, VkFormat imageFormat, int32_t texWidth, int32_t texHeight, uint32_t mipLevels);
        VkSampleCountFlagBits getMaxUsableSampleCount();
        void copyBuffer(VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
        void useRenderObject(RenderObject& renderObject); 
    private: 
        void cleanupSwapChain();
        void reCreateSwapChain();
        void drawFrame();
};

#endif
