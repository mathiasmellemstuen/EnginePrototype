#ifndef ENGINEPROTOTYPE_PHYSICALDEVICE
#define ENGINEPROTOTYPE_PHYSICALDEVICE

    #define GLFW_INCLUDE_VULKAN
    #include <GLFW/glfw3.h>
    #include <optional>
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
            PhysicalDevice(VulkanInstance& instance);
            ~PhysicalDevice();
            QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        private:
            bool isDeviceSuitable(VkPhysicalDevice device);
            VkSurfaceKHR* surface;
    };
#endif