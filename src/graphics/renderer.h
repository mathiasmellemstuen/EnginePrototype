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

#include <vector>

class Renderer {
    public:
        Window& window; 
        VulkanInstance vulkanInstance;
        PhysicalDevice physicalDevice; 
        LogicalDevice logicalDevice;
        SwapChain swapChain;
        ImageViews imageViews;
        Shader shader;
        DescriptorSetLayout descriptorSetLayout; 
        GraphicsPipeline graphicsPipeline;
        CommandPool commandPool;
        ColorResources colorResources;
        DepthResources depthResources;
        FrameBuffers frameBuffers;
        Texture texture; 
        VertexBuffer vertexBuffer;
        UniformBuffer uniformBuffer; 
        DescriptorPool descriptorPool;
        CommandBuffers commandBuffers;
        SyncObjects syncObjects;

        Renderer(Window& window, std::vector<Vertex>& verticies, std::vector<uint32_t>& indices);
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
    private: 
        void cleanupSwapChain();
        void reCreateSwapChain();
        void drawFrame();


};

#endif