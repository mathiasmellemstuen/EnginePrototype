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

const Texture& createTexture(Renderer& renderer, const std::string& texturePath);
void freeTexture(Renderer& renderer, Texture& texture);
void createTextureImageView(Renderer& renderer, Texture& texture);
void createTextureSampler(Renderer& renderer, Texture& texture);
void transitionImageLayout(Renderer& renderer, VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
void copyBufferToImage(Renderer& renderer, VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
#endif
