#ifndef ENGINEPROTOTYPE_TEXTURE
#define ENGINEPROTOTYPE_TEXTURE

#include <vulkan/vulkan.h>
#include "logicalDevice.h"
#include "vertexBuffer.h"
#include "physicalDevice.h"
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
        void copyBufferToImage(LogicalDevice& logicalDevice, CommandPool& commandPool, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
        void transitionImageLayout(LogicalDevice& logicalDevice, CommandPool& commandPool, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout);
        void createTextureImageView(LogicalDevice& logicalDevice, ImageViews& imageViews);
        void createTextureSampler(LogicalDevice& logicalDevice);
        void create(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, CommandPool& commandPool, ImageViews& imageViews); 
        Texture(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, CommandPool& commandPool, ImageViews& imageViews); 
        ~Texture();
};
#endif