#ifndef ENGINEPROTOTYPE_VULKANINSTANCE
#define ENGINEPROTOTYPE_VULKANINSTANCE 
#define GLFW_INCLUDE_VULKAN

#include <GLFW/glfw3.h>

class VulkanInstance {
    public: 
        VkInstance instance;
        VulkanInstance(); 
        ~VulkanInstance(); 
};

#endif