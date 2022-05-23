#ifndef ENGINEPROTOTYPE_LOGICALDEVICE
#define ENGINEPROTOTYPE_LOGICALDEVICE

#include <vulkan/vulkan.h>

class Renderer;

class LogicalDevice {
    public: 
        VkDevice device;
        VkQueue graphicsQueue;
        VkQueue presentQueue;
        void create(); 
        LogicalDevice(Renderer& renderer);
        void clean();
    private:
        Renderer& renderer;
};


#endif