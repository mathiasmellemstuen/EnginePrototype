#ifndef ENGINEPROTOTYPE_TEXTURE
#define ENGINEPROTOTYPE_TEXTURE

#include <vulkan/vulkan.h>
#include <string>
#include "renderer.h"

struct Texture {
    std::string texturePath; 
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    uint32_t mipLevels;
    VkImage textureImage;
    VkDeviceMemory textureImageMemory;
    VkImageView textureImageView;
    VkSampler textureSampler;
};

Texture createTexture(RendererContent& rendererContent, const std::string& texturePath);
void freeTexture(RendererContent& rendererContent, Texture& texture);
void createTextureImageView(RendererContent& rendererContent, Texture& texture);
void createTextureSampler(RendererContent& rendererContent, Texture& texture);
void transitionImageLayout(RendererContent& rendererContent, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
void copyBufferToImage(RendererContent& rendererContent, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
#endif