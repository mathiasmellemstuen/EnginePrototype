#ifndef ENGINEPROTOTYPE_COMMANDPOOL
#define ENGINEPROTOTYPE_COMMANDPOOL

#include <vulkan/vulkan.h>

class Renderer;

class CommandPool {
    public: 
        VkCommandPool commandPool; 
        void create(); 
        CommandPool(Renderer& renderer); 
        void clean(); 
    private:
        Renderer& renderer;
};

#endif