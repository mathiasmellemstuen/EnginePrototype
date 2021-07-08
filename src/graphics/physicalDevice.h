#ifndef ENGINEPROTOTYPE_PHYSICALDEVICE
#define ENGINEPROTOTYPE_PHYSICALDEVICE

#include <vulkan/vulkan.h>
#include <optional>
#include <vector>
#include "vulkanInstance.h"

struct QueueFamilyIndices {
    std::optional<uint32_t> graphicsFamily;
    std::optional<uint32_t> presentFamily;

    bool isComplete() { 
        return graphicsFamily.has_value() && presentFamily.has_value();
    }
};


class PhysicalDevice {
    public:
        VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
        VkPhysicalDeviceProperties physicalDeviceProperties; 
        PhysicalDevice(VulkanInstance& instance);
        ~PhysicalDevice();
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        VkSurfaceKHR* surface;
    private:
        bool checkDeviceExtensionSupport(VkPhysicalDevice device);
        bool isDeviceSuitable(VkPhysicalDevice device);
};
#endif