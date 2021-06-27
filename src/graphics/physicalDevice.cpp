#include "physicalDevice.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>

PhysicalDevice::PhysicalDevice(VkInstance& instance) {    
    uint32_t deviceCount = 0; 
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);
    
};

PhysicalDevice::~PhysicalDevice() {

};