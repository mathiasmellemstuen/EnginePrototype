#ifndef ENGINEPROTOTYPE_PHYSICALDEVICE
#define ENGINEPROTOTYPE_PHYSICALDEVICE

    #define GLFW_INCLUDE_VULKAN
    #include <GLFW/glfw3.h>
    #include <optional>

    struct QueueFamilyIndices {
        std::optional<uint32_t> graphicsFamily;

        bool isComplete() {
            return graphicsFamily.has_value();
        }
    };

    class PhysicalDevice {
        public: 
            VkPhysicalDevice physicalDevice = VK_NULL_HANDLE;
            PhysicalDevice(VkInstance& instance);
            ~PhysicalDevice();
            QueueFamilyIndices findQueueFamilies(VkPhysicalDevice device);
        private:
            bool isDeviceSuitable(VkPhysicalDevice device);
    };
#endif