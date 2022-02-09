#ifndef ENGINEPROTOTYPE_IMAGEVIEWS
#define ENGINEPROTOTYPE_IMAGEVIEWS

#include <vulkan/vulkan.h>
#include <iostream>
#include <vector>

class Renderer;

class ImageViews {
    public: 
        std::vector<VkImageView> swapChainImageViews;
        ImageViews(Renderer& renderer); 
        void clean();
        void create(); 
        VkImageView createImageView(VkImage image, VkFormat format, VkImageAspectFlags aspectFlags, uint32_t mipLevels);
    private:
        Renderer& renderer;
};

#endif