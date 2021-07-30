#ifndef ENGINEPROTOTYPE_TEXTURE
#define ENGINEPROTOTYPE_TEXTURE

#include <vulkan/vulkan.h>
#include "logicalDevice.h"
#include "vertexBuffer.h"
#include "physicalDevice.h"
#include "commandBuffer.h"
#include "commandPool.h"
#include "imageViews.h"

class Texture {
    public:
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        VkImage textureImage;
        VkDeviceMemory textureImageMemory;
        VkImageView textureImageView;
        VkSampler textureSampler;
        VkDevice* device; 
        void copyBufferToImage(LogicalDevice& logicalDevice, CommandPool& commandPool, CommandBuffers& commandBuffers, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
        void transitionImageLayout(LogicalDevice& logicalDevice, CommandPool& commandPool, CommandBuffers& commandBuffers, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
        void createImage(PhysicalDevice& physicalDevice, VertexBuffer& vertexBuffer, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);
        void createTextureImageView(LogicalDevice& logicalDevice, ImageViews& imageViews);
        void createTextureSampler(LogicalDevice& logicalDevice);
        void create(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, VertexBuffer& vertexBuffer, CommandPool& commandPool, CommandBuffers& commandBuffers, ImageViews& imageViews); 
        Texture(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, VertexBuffer& vertexBuffer, CommandPool& commandPool, CommandBuffers& commandBuffers, ImageViews& imageViews); 
        ~Texture();
};
#endif