#ifndef ENGINEPROTOTYPE_PHYSICALDEVICE
#define ENGINEPROTOTYPE_PHYSICALDEVICE

#include <vulkan/vulkan.h>
#include <optional>
#include <vector>

class Renderer;

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
        PhysicalDevice(Renderer& renderer);
        void clean();
        QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        VkSurfaceKHR* surface;
        VkSampleCountFlagBits msaaSamples = VK_SAMPLE_COUNT_1_BIT;
    private:
        bool checkDeviceExtensionSupport(VkPhysicalDevice device);
        bool isDeviceSuitable(VkPhysicalDevice device);
        Renderer& renderer;
};
#endif