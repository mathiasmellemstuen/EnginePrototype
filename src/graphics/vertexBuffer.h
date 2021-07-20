#ifndef ENGINEPROTOTYPE_VERTEXBUFFER
#define ENGINEPROTOTYPE_VERTEXBUFFER

#include <vulkan/vulkan.h>
#include "logicalDevice.h"
#include "physicalDevice.h" 
#include "vertex.h"
#include <vector>

class VertexBuffer {
    public:
        VkDevice* device;
        VkBuffer buffer;
        VkDeviceMemory vertexBufferMemory;
        std::vector<Vertex> vertices;
        void create(PhysicalDevice& physicalDevice, std::vector<Vertex>& vertices);
        void cleanup(); 
        VertexBuffer(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, std::vector<Vertex>& vertices);
        ~VertexBuffer();
    private:
        uint32_t findMemoryType(PhysicalDevice& physicalDevice, uint32_t typeFilter, VkMemoryPropertyFlags properties);
        void createBuffer(PhysicalDevice& physicalDevice, VkDeviceSize size, VkBufferUsageFlags usage, VkMemoryPropertyFlags properties);
};
#endif