#include "physicalDevice.h"

#define GLFW_INCLUDE_VULKAN
#include <GLFW/glfw3.h>
#include <stdexcept>
#include <vector>
#include <optional>
#include <set>
#include "swapChainSupport.h"
#include "deviceExtensions.h"

#include "vulkanInstance.h"

bool PhysicalDevice::checkDeviceExtensionSupport(VkPhysicalDevice device) {
    
    uint32_t extensionCount; 
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, nullptr); 
    
    std::vector<VkExtensionProperties> availableExtensions(extensionCount); 
    vkEnumerateDeviceExtensionProperties(device, nullptr, &extensionCount, availableExtensions.data()); 
    
    std::set<std::string> requiredExtensions(deviceExtensions.begin(), deviceExtensions.end()); 
    
    for (const auto& extension : availableExtensions) { 
        requiredExtensions.erase(extension.extensionName); 
    } 
     
    return requiredExtensions.empty();
};
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

        
        VkBool32 presentSupport = false;
        vkGetPhysicalDeviceSurfaceSupportKHR(device, i, *surface, &presentSupport);
         
        if (presentSupport) {
            indices.presentFamily = i;
        }

        i++; 
    }
    return indices; 
}

bool PhysicalDevice::isDeviceSuitable(VkPhysicalDevice device) {
    
    QueueFamilyIndices indices = findQueueFamilies(device); 
    bool extensionsSupported = checkDeviceExtensionSupport(device); 

    bool swapChainAdequate = false; 
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport =  querySwapChainSupport(device, *surface); 
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty(); 
    }

    return indices.isComplete() && extensionsSupported && swapChainAdequate; 
};



PhysicalDevice::PhysicalDevice(VulkanInstance& instance) {  
    surface = &instance.surface;

    uint32_t deviceCount = 0; 
    vkEnumeratePhysicalDevices(instance.instance, &deviceCount, nullptr);

    if (deviceCount == 0) { 
        throw std::runtime_error("Failed to find GPUs with Vulkan support!"); 
    }

    std::vector<VkPhysicalDevice> devices(deviceCount); 
    vkEnumeratePhysicalDevices(instance.instance, &deviceCount, devices.data());

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