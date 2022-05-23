#ifndef ENGINEPROTOTYPE_DEPTHRESOURCES
#define ENGINEPROTOTYPE_DEPTHRESOURCES

#include <vulkan/vulkan.h>

class Renderer;

class DepthResources {
    public:
        VkImage depthImage;
        VkDeviceMemory depthImageMemory;
        VkImageView depthImageView;
        void create(); 
        DepthResources(Renderer& renderer); 
        void clean();
    private:
        Renderer& renderer;
};

#endif