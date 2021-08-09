#ifndef ENGINEPROTOTYPE_COLORRESOURCES
#define ENGINEPROTOTYPE_COLORRESOURCES

#include <vulkan/vulkan.h>

class Renderer;

class ColorResources {
    public:
        VkImage colorImage;
        VkDeviceMemory colorImageMemory;
        VkImageView colorImageView;
        void create();
        ColorResources(Renderer& renderer);
        ~ColorResources();
    private:
        Renderer& renderer;
};

#endif