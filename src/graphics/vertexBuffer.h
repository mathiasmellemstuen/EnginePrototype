#ifndef ENGINEPROTOTYPE_VERTEXBUFFER
#define ENGINEPROTOTYPE_VERTEXBUFFER

#include <vulkan/vulkan.h>
#include "logicalDevice.h"
#include "physicalDevice.h" 
#include "vertex.h"
#include <vector>
#include "commandPool.h"

class VertexBuffer {
    public:
        VkDevice* device;
        VkBuffer vertexBuffer;
        VkBuffer indexBuffer;
        VkDeviceMemory vertexBufferMemory;
        VkDeviceMemory indexBufferMemory;
        std::vector<Vertex> vertices;
        std::vector<uint16_t> indices;
        void cleanup(); 
        VertexBuffer(PhysicalDevice& physicalDevice, CommandPool& commandPool, LogicalDevice& logicalDevice, std::vector<Vertex>& vertices, std::vector<uint16_t>& indices);
        ~VertexBuffer();
    private:
        void createVertexBuffer(PhysicalDevice& physicalDevice, CommandPool& commandPool, LogicalDevice& logicalDevice, std::vector<Vertex>& vertices);
        void createIndexBuffer(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, CommandPool& commandPool, std::vector<uint16_t>& indices); 
        void copyBuffer(LogicalDevice& logicalDevice, CommandPool& commandPool, VkBuffer srcBuffer, VkBuffer dstBuffer, VkDeviceSize size);
};
#endif