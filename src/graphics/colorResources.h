#ifndef ENGINEPROTOTYPE_COLORRESOURCES
#define ENGINEPROTOTYPE_COLORRESOURCES

#include <vulkan/vulkan.h>
#include "physicalDevice.h"
#include "logicalDevice.h"
#include "swapChain.h"
#include "imageViews.h"

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