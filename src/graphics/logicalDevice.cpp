
#include "logicalDevice.h"

#include <vulkan/vulkan.h>
#include <stdexcept>
#include <set>

#include "physicalDevice.h"
#include "validationLayers.h"
#include "deviceExtensions.h"
#include "../utility/debug.h"

#include "renderer.h"

LogicalDevice::LogicalDevice(Renderer& renderer) : renderer(renderer) {
    create(); 
};

void LogicalDevice::create() {
    Debug::log(INFO, "Setting up a logical device"); 

    QueueFamilyIndices indices = renderer.physicalDevice.findQueueFamilies(renderer.physicalDevice.physicalDevice);

    std::vector<VkDeviceQueueCreateInfo> queueCreateInfos;
    std::set<uint32_t> uniqueQueueFamilies = {indices.graphicsFamily.value(), indices.presentFamily.value()};
   
    VkDeviceQueueCreateInfo queueCreateInfo{};
    queueCreateInfo.sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO;
    queueCreateInfo.queueFamilyIndex = indices.graphicsFamily.value();
    queueCreateInfo.queueCount = 1;
    
    float queuePriority = 1.0f;

    for (uint32_t queueFamily : uniqueQueueFamilies) {
        VkDeviceQueueCreateInfo queueCreateInfo{};
        queueCreateInfo.sType =  VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO; 
        queueCreateInfo.queueFamilyIndex = queueFamily;
        queueCreateInfo.queueCount = 1;
        queueCreateInfo.pQueuePriorities = &queuePriority;
        queueCreateInfos.push_back(queueCreateInfo);
    }

    VkPhysicalDeviceFeatures deviceFeatures{};
    deviceFeatures.samplerAnisotropy = VK_TRUE; 
    
    VkDeviceCreateInfo createInfo{};
    createInfo.sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO;
    createInfo.pQueueCreateInfos = &queueCreateInfo;
    createInfo.queueCreateInfoCount = 1;
    createInfo.pEnabledFeatures = &deviceFeatures;

    createInfo.enabledExtensionCount =  static_cast<uint32_t>(deviceExtensions.size()); 
    createInfo.ppEnabledExtensionNames = deviceExtensions.data();

    createInfo.queueCreateInfoCount = static_cast<uint32_t>(queueCreateInfos.size()); 
    createInfo.pQueueCreateInfos = queueCreateInfos.data();

    if (enableValidationLayers) {
        createInfo.enabledLayerCount = 
        static_cast<uint32_t>(validationLayers.size());
        createInfo.ppEnabledLayerNames = validationLayers.data(); 
    } else { 
        createInfo.enabledLayerCount = 0; 
    }

    if (vkCreateDevice(renderer.physicalDevice.physicalDevice, &createInfo, nullptr, &device) !=  VK_SUCCESS) { 
        
        Debug::log(ERROR, "Failed to create logical device!"); 
        throw std::runtime_error("Failed to create logical device!"); 
    }

    vkGetDeviceQueue(device, indices.graphicsFamily.value(), 0,  &graphicsQueue);
    vkGetDeviceQueue(device, indices.presentFamily.value(), 0,  &presentQueue);

    Debug::log(SUCCESS, "Created logical device!"); 
}

LogicalDevice::~LogicalDevice() {
    Debug::log(INFO, "Destroying logical device"); 
    vkDestroyDevice(device, nullptr);
    Debug::log(SUCCESS, "Logical device destroyed"); 
};