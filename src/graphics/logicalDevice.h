#ifndef ENGINEPROTOTYPE_LOGICALDEVICE
#define ENGINEPROTOTYPE_LOGICALDEVICE

#include <vulkan/vulkan.h>
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