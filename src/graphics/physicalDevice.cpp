#include "physicalDevice.h"

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <vector>
#include <optional>
#include <set>
#include "swapChainSupport.h"
#include "deviceExtensions.h"
#include <iostream>
#include "../utility/debug.h"

#include "vulkanInstance.h"

#include "renderer.h"

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
    
    Debug::log(INFO, "Checking if device is suitable."); 

    QueueFamilyIndices indices = findQueueFamilies(device); 
    bool extensionsSupported = checkDeviceExtensionSupport(device); 

    bool swapChainAdequate = false; 
    if (extensionsSupported) {
        SwapChainSupportDetails swapChainSupport =  querySwapChainSupport(device, *surface); 
        swapChainAdequate = !swapChainSupport.formats.empty() && !swapChainSupport.presentModes.empty(); 
    }

    VkPhysicalDeviceFeatures supportedFeatures;
    vkGetPhysicalDeviceFeatures(device, &supportedFeatures);


    return indices.isComplete() && extensionsSupported && swapChainAdequate && supportedFeatures.samplerAnisotropy; 
};



PhysicalDevice::PhysicalDevice(Renderer& renderer) : renderer(renderer) {

    Debug::log(INFO, "Checking and creating physical device context"); 
    surface = &renderer.vulkanInstance.surface;

    uint32_t deviceCount = 0; 
    vkEnumeratePhysicalDevices(renderer.vulkanInstance.instance, &deviceCount, nullptr);

    if (deviceCount == 0) { 
        Debug::log(ERROR, "Failed to find GPUs with Vulkan support!"); 
        throw std::runtime_error("Failed to find GPUs with Vulkan support!"); 
    }

    std::vector<VkPhysicalDevice> devices(deviceCount); 
    vkEnumeratePhysicalDevices(renderer.vulkanInstance.instance, &deviceCount, devices.data());

    for (const auto& device : devices) {
        if (isDeviceSuitable(device)) { 
            physicalDevice = device;
            msaaSamples = renderer.getMaxUsableSampleCount();
            break; 
        }
    } 

    if (physicalDevice == VK_NULL_HANDLE) { 
        
        Debug::log(ERROR, "Failed to find a suitable GPU!"); 
        throw std::runtime_error("Failed to find a suitable GPU!");
  
    } else {
    
        vkGetPhysicalDeviceProperties(physicalDevice,&physicalDeviceProperties);
        Debug::log(SUCCESS, "Selected GPU: " + std::string(physicalDeviceProperties.deviceName) + ". Successfully created PhysicalDevice context.");
    }
    
};

PhysicalDevice::~PhysicalDevice() {

};
