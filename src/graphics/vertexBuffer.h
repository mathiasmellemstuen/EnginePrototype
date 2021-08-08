#ifndef ENGINEPROTOTYPE_VERTEXBUFFER
#define ENGINEPROTOTYPE_VERTEXBUFFER

#include <vulkan/vulkan.h>
#include "logicalDevice.h"
#include "physicalDevice.h" 
#include "vertex.h"
#include <vector>
#include "commandPool.h"

class Renderer;

class VertexBuffer {
    public:
        VkBuffer vertexBuffer;
        VkBuffer indexBuffer;
        VkDeviceMemory vertexBufferMemory;
        VkDeviceMemory indexBufferMemory;
        std::vector<Vertex> vertices;
        std::vector<uint32_t> indices;
        void cleanup(); 
        VertexBuffer(Renderer& renderer, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
        ~VertexBuffer();
    private:
        Renderer& renderer;
        void createVertexBuffer(std::vector<Vertex>& vertices);
        void createIndexBuffer(std::vector<uint32_t>& indices); 
};
#endif