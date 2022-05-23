#ifndef ENGINEPROTOTYPE_VULKANINSTANCE
#define ENGINEPROTOTYPE_VULKANINSTANCE 

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

class Renderer;

class VulkanInstance {
    public: 
        VkInstance instance;
        VkSurfaceKHR surface;
        VulkanInstance(Renderer& renderer); 
        void clean(); 
    private:
        Renderer& renderer;
};

#endif