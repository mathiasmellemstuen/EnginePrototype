#ifndef ENGINEPROTOTYPE_ENGINESTATIC
#define ENGINEPROTOTYPE_ENGINESTATIC

#include "physicalDevice.h"
#include "logicalDevice.h"
#include <vulkan/vulkan.h>
#include "commandPool.h"
#include <vector>

void createBuffer(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties, VkBuffer& buffer, VkDeviceMemory& bufferMemory);
uint32_t findMemoryType(PhysicalDevice& physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);
VkCommandBuffer beginSingleTimeCommands(LogicalDevice& logicalDevice, CommandPool& commandPool);
void endSingleTimeCommands(LogicalDevice& logicalDevice, CommandPool& commandPool, VkCommandBuffer commandBuffer);
VkFormat findSupportedFormat(PhysicalDevice& physicalDevice, const std::vector<VkFormat>&candidates, VkImageTiling tiling, VkFormatFeatureFlags features);
VkFormat findDepthFormat(PhysicalDevice& physicalDevice);
bool hasStencilComponent(VkFormat format);
void createImage(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, uint32_t width, uint32_t height, VkFormat format, VkImageTiling tiling, VkImageUsageFlags usage, VkMemoryPropertyFlags properties, VkImage& image, VkDeviceMemory& imageMemory);

#endif