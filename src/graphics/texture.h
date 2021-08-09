#ifndef ENGINEPROTOTYPE_TEXTURE
#define ENGINEPROTOTYPE_TEXTURE

#include <vulkan/vulkan.h>

class Renderer;

class Texture {
    public:
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        uint32_t mipLevels;
        VkImage textureImage;
        VkDeviceMemory textureImageMemory;
        VkImageView textureImageView;
        VkSampler textureSampler;
        void copyBufferToImage(VkBuffer buffer, VkImage image, uint32_t width, uint32_t height);
        void transitionImageLayout(VkImage image, VkFormat format, VkImageLayout oldLayout, VkImageLayout newLayout, uint32_t mipLevels);
        void createTextureImageView(uint32_t mipLevels);
        void createTextureSampler();
        void create(); 
        Texture(Renderer& renderer); 
        ~Texture();
    private:
        Renderer& renderer;
};
#endif