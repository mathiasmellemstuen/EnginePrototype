#include "commandPool.h"
#include <vulkan/vulkan.h>
#include "physicalDevice.h"
#include "../utility/log.h"
#include "logicalDevice.h"
#include <stdexcept>

CommandPool::CommandPool(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice) {
    this->device = &logicalDevice.device; 
    create(physicalDevice); 
}
void CommandPool::create(PhysicalDevice& physicalDevice) {

    log(INFO, "Creating command pool."); 

    QueueFamilyIndices queueFamilyIndices = physicalDevice.findQueueFamilies(physicalDevice.physicalDevice);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value(); 
    poolInfo.flags = 0; // Optional

    if (vkCreateCommandPool(*device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
        log(ERROR, "Failed to create command pool!"); 
        throw std::runtime_error("failed to create command pool!");
    }
    log(SUCCESS, "Command pool created!"); 
}

CommandPool::~CommandPool() {
    vkDestroyCommandPool(*device, commandPool, nullptr);
}