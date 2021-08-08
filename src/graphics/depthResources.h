#ifndef ENGINEPROTOTYPE_DEPTHRESOURCES
#define ENGINEPROTOTYPE_DEPTHRESOURCES

#include <vulkan/vulkan.h>
#include "physicalDevice.h"
#include "imageViews.h"
#include "texture.h"
#include "swapChain.h"
#include "logicalDevice.h"

class Renderer;

class DepthResources {
    public:
        VkImage depthImage;
        VkDeviceMemory depthImageMemory;
        VkImageView depthImageView;
        void create(); 
        DepthResources(Renderer& renderer); 
        ~DepthResources();
    private:
        Renderer& renderer;
};

#endif