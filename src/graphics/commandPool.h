#ifndef ENGINEPROTOTYPE_COMMANDPOOL
#define ENGINEPROTOTYPE_COMMANDPOOL

#include <vulkan/vulkan.h>
#include "physicalDevice.h"
#include "logicalDevice.h"

class CommandPool {
    public: 
        VkCommandPool commandPool;
        VkDevice* device; 
        void create(PhysicalDevice& physicalDevice); 
        CommandPool(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice); 
        ~CommandPool(); 
};

#endif