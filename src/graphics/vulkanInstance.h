#ifndef ENGINEPROTOTYPE_VULKANINSTANCE
#define ENGINEPROTOTYPE_VULKANINSTANCE 
#define GLFW_INCLUDE_VULKAN

#include <SDL2/SDL.h>
#include <SDL2/SDL_vulkan.h>

class VulkanInstance {
    public: 
        VkInstance instance;
        VkSurfaceKHR surface;
        VulkanInstance(SDL_Window& window); 
        ~VulkanInstance(); 
};

#endif