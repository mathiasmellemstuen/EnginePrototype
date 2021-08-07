#include "vertexBuffer.h"
#include <stdexcept>
#include <cstring>
#include "../utility/debug.h"
#include <iostream>
#include "commandPool.h"
#include "vulkanHelperFunctions.h"

VertexBuffer::VertexBuffer(PhysicalDevice& physicalDevice, CommandPool& commandPool, LogicalDevice& logicalDevice, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices) {
    this->device = &logicalDevice.device; 
    this->vertices = vertices;
    this->indices = indices;

    createVertexBuffer(physicalDevice, commandPool, logicalDevice, vertices);
    createIndexBuffer(physicalDevice, logicalDevice, commandPool, indices);
};

void VertexBuffer::createVertexBuffer(PhysicalDevice& physicalDevice, CommandPool& commandPool, LogicalDevice& logicalDevice, std::vector<Vertex>& vertices) {
    Debug::log(INFO, "Creating vertex buffer.");

    
    VkDeviceSize bufferSize = sizeof(this->vertices[0]) * this->vertices.size();
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(physicalDevice, logicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
    
    void* data;
    vkMapMemory(*device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, this->vertices.data(), (size_t) bufferSize);
    vkUnmapMemory(*device, stagingBufferMemory);

    createBuffer(physicalDevice, logicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);


    copyBuffer(logicalDevice, commandPool, stagingBuffer, vertexBuffer, bufferSize);

    vkDestroyBuffer(*device, stagingBuffer, nullptr);
    vkFreeMemory(*device, stagingBufferMemory, nullptr);

    Debug::log(SUCCESS, "Vertex buffer created!"); 
};

void VertexBuffer::createIndexBuffer(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, CommandPool& commandPool, std::vector<uint32_t>& indices) {
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    createBuffer(physicalDevice, logicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(*device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t) bufferSize);
    vkUnmapMemory(*device, stagingBufferMemory);

    createBuffer(physicalDevice, logicalDevice, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

    copyBuffer(logicalDevice, commandPool, stagingBuffer, indexBuffer, bufferSize);
    vkDestroyBuffer(*device, stagingBuffer, nullptr);
    vkFreeMemory(*device, stagingBufferMemory, nullptr);
}
void VertexBuffer::copyBuffer(LogicalDevice& logicalDevice, CommandPool& commandPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size) {
    
    VkCommandBufferAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_ALLOCATE_INFO;
    allocInfo.level = VK_COMMAND_BUFFER_LEVEL_PRIMARY;
    allocInfo.commandPool = commandPool.commandPool;
    allocInfo.commandBufferCount = 1;
    VkCommandBuffer commandBuffer;
    vkAllocateCommandBuffers(*device, &allocInfo, &commandBuffer);

    VkCommandBufferBeginInfo beginInfo{};
    beginInfo.sType = VK_STRUCTURE_TYPE_COMMAND_BUFFER_BEGIN_INFO;
    beginInfo.flags = VK_COMMAND_BUFFER_USAGE_ONE_TIME_SUBMIT_BIT;
    vkBeginCommandBuffer(commandBuffer, &beginInfo);

    VkBufferCopy copyRegion{};
    copyRegion.srcOffset = 0; // Optional
    copyRegion.dstOffset = 0; // Optional
    copyRegion.size = size;
    vkCmdCopyBuffer(commandBuffer, srcBuffer, dstBuffer, 1, &copyRegion);

    vkEndCommandBuffer(commandBuffer);

    VkSubmitInfo submitInfo{};
    submitInfo.sType = VK_STRUCTURE_TYPE_SUBMIT_INFO;
    submitInfo.commandBufferCount = 1;
    submitInfo.pCommandBuffers = &commandBuffer;

    vkQueueSubmit(logicalDevice.graphicsQueue, 1, &submitInfo, VK_NULL_HANDLE);
    vkQueueWaitIdle(logicalDevice.graphicsQueue);

    vkFreeCommandBuffers(*device, commandPool.commandPool, 1, &commandBuffer);
}

void VertexBuffer::cleanup() {
    
    vkDestroyBuffer(*device, indexBuffer, nullptr);
    vkFreeMemory(*device, indexBufferMemory, nullptr);

    vkDestroyBuffer(*device, vertexBuffer, nullptr);
    vkFreeMemory(*device, vertexBufferMemory, nullptr);

}
VertexBuffer::~VertexBuffer() {
    cleanup();
};