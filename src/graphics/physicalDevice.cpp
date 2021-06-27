#include "physicalDevice.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <vector>
#include <optional>

QueueFamilyIndices PhysicalDevice::findQueueFamilies(VkPhysicalDevice device) {
    QueueFamilyIndices indices;

    uint32_t queueFamilyCount = 0;
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount,  nullptr); 
 
    std::vector<VkQueueFamilyProperties> queueFamilies(queueFamilyCount); 
    vkGetPhysicalDeviceQueueFamilyProperties(device, &queueFamilyCount, queueFamilies.data());

    int i = 0;
    for (const auto& queueFamily : queueFamilies) {

        if (indices.isComplete()) {
            break;
        }

        if (queueFamily.queueFlags & VK_QUEUE_GRAPHICS_BIT) { 
            indices.graphicsFamily = i; 
        } 
 
        i++; 
    }
    return indices; 
}

bool PhysicalDevice::isDeviceSuitable(VkPhysicalDevice device) {
    
    QueueFamilyIndices indices = findQueueFamilies(device);

    return indices.isComplete();
};

PhysicalDevice::PhysicalDevice(VkInstance& instance) {    
    uint32_t deviceCount = 0; 
    vkEnumeratePhysicalDevices(instance, &deviceCount, nullptr);

    if (deviceCount == 0) { 
        throw std::runtime_error("Failed to find GPUs with Vulkan support!"); 
    }

    std::vector<VkPhysicalDevice> devices(deviceCount); 
    vkEnumeratePhysicalDevices(instance, &deviceCount, devices.data());

    for (const auto& device : devices) {
        if (isDeviceSuitable(device)) { 
            physicalDevice = device; 
            break; 
        }
    } 

    if (physicalDevice == VK_NULL_HANDLE) { 
        throw std::runtime_error("Failed to find a suitable GPU!");
    }
};

PhysicalDevice::~PhysicalDevice() {

};