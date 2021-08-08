#ifndef ENGINEPROTOTYPE_COMMANDPOOL
#define ENGINEPROTOTYPE_COMMANDPOOL

#include <vulkan/vulkan.h>
#include "physicalDevice.h"
#include "logicalDevice.h"

class Renderer;

class CommandPool {
    public: 
        VkCommandPool commandPool; 
        void create(); 
        CommandPool(Renderer& renderer); 
        ~CommandPool(); 
    private:
        Renderer& renderer;
};

#endif