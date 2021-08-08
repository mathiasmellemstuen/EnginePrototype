#include "commandPool.h"
#include <vulkan/vulkan.h>
#include "physicalDevice.h"
#include "../utility/debug.h"
#include "logicalDevice.h"
#include <stdexcept>

#include "renderer.h"

CommandPool::CommandPool(Renderer& renderer) : renderer(renderer) {
    create();
}
void CommandPool::create() {

    Debug::log(INFO, "Creating command pool."); 

    QueueFamilyIndices queueFamilyIndices = renderer.physicalDevice.findQueueFamilies(renderer.physicalDevice.physicalDevice);

    VkCommandPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO;
    poolInfo.queueFamilyIndex = queueFamilyIndices.graphicsFamily.value(); 
    poolInfo.flags = 0; // Optional

    if (vkCreateCommandPool(renderer.logicalDevice.device, &poolInfo, nullptr, &commandPool) != VK_SUCCESS) {
        Debug::log(ERROR, "Failed to create command pool!"); 
        throw std::runtime_error("failed to create command pool!");
    }
    Debug::log(SUCCESS, "Command pool created!"); 
}

CommandPool::~CommandPool() {
    vkDestroyCommandPool(renderer.logicalDevice.device, commandPool, nullptr);
}