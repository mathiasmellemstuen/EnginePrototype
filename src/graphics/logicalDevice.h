#ifndef ENGINEPROTOTYPE_LOGICALDEVICE
#define ENGINEPROTOTYPE_LOGICALDEVICE

    #define GLFW_INCLUDE_VULKAN
    #include <GLFW/glfw3.h>

    #include "physicalDevice.h"

    class LogicalDevice {
        public: 
            VkDevice device;
            VkQueue graphicsQueue;
            VkQueue presentQueue;
            LogicalDevice(PhysicalDevice& physicalDevice);
            ~LogicalDevice();
    };


#endif