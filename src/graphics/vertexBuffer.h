#ifndef ENGINEPROTOTYPE_VERTEXBUFFER
#define ENGINEPROTOTYPE_VERTEXBUFFER

#include <vulkan/vulkan.h>
#include "vertex.h"
#include <vector>
#include "renderer.h"

struct VertexBuffer {
    VkBuffer vertexBuffer;
    VkBuffer indexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkDeviceMemory indexBufferMemory;
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
};
const VertexBuffer& createVertexBuffer(RendererContent& rendererContent, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices);
void freeVertexBuffer(RendererContent& rendererContent, VertexBuffer& vertexBuffer);

#endif