#include "vertexBuffer.h"
#include <stdexcept>
#include <cstring>
#include "../utility/log.h"
#include <iostream>

VertexBuffer::VertexBuffer(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, std::vector<Vertex>& vertices) {
    this->device = &logicalDevice.device; 
    create(physicalDevice, vertices);
};

void VertexBuffer::create(PhysicalDevice& physicalDevice, std::vector<Vertex>& vertices) {
    this->vertices = vertices;  

    VkDeviceSize bufferSize = sizeof(this->vertices[0]) * this->vertices.size();
    createBuffer(physicalDevice, bufferSize, VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);
    
    void* data;
    vkMapMemory(*device, vertexBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, this->vertices.data(), (size_t) bufferSize);
    vkUnmapMemory(*device, vertexBufferMemory);
};

void VertexBuffer::createBuffer(PhysicalDevice& physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties) {
    VkBufferCreateInfo bufferInfo{};
    bufferInfo.sType = VK_STRUCTURE_TYPE_BUFFER_CREATE_INFO;
    bufferInfo.size = sizeof(vertices[0]) * vertices.size();
    bufferInfo.usage = VK_BUFFER_USAGE_VERTEX_BUFFER_BIT;
    bufferInfo.sharingMode = VK_SHARING_MODE_EXCLUSIVE;

    if (vkCreateBuffer(*device, &bufferInfo, nullptr, &buffer) != VK_SUCCESS) {
        throw std::runtime_error("failed to create vertex buffer!");
    }

    VkMemoryRequirements memRequirements;
    vkGetBufferMemoryRequirements(*device, buffer, &memRequirements);

    VkMemoryAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO;
    allocInfo.allocationSize = memRequirements.size;
    allocInfo.memoryTypeIndex = findMemoryType(physicalDevice, memRequirements.memoryTypeBits, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT);

    if (vkAllocateMemory(*device, &allocInfo, nullptr, &vertexBufferMemory) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate vertex buffer memory!");
    }
    
    vkBindBufferMemory(*device, buffer, vertexBufferMemory, 0);
}
uint32_t VertexBuffer::findMemoryType(PhysicalDevice& physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties) {
    VkPhysicalDeviceMemoryProperties memProperties;
    vkGetPhysicalDeviceMemoryProperties(physicalDevice.physicalDevice, &memProperties);

    for (uint32_t i = 0; i < memProperties.memoryTypeCount; i++) {
        if (typeFilter & (1 << i) && (memProperties.memoryTypes[i].propertyFlags & properties) == properties) {
            return i;
        }
    }

    throw std::runtime_error("failed to find suitable memory type!");
};
void VertexBuffer::cleanup() {
    
    vkDestroyBuffer(*device, buffer, nullptr);
    vkFreeMemory(*device, vertexBufferMemory, nullptr);

}
VertexBuffer::~VertexBuffer() {
    cleanup();
};