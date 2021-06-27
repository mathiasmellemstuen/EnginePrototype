#ifndef ENGINEPROTOTYPE_PHYSICALDEVICE
#define ENGINEPROTOTYPE_PHYSICALDEVICE

    #define GLFW_INCLUDE_VULKAN
    #include <GLFW/glfw3.h>

    class PhysicalDevice {
        public: 
            VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
            PhysicalDevice(VkInstance& instance);
            ~PhysicalDevice();
    };
#endif