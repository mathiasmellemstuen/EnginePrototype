#include "vertexBuffer.h"
#include "renderer.h"
#include "../utility/debug.h"

VertexBuffer createVertexBuffer(RendererContent& rendererContent, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices) {
    VertexBuffer vertexBuffer; 
    vertexBuffer.vertices = vertices; 
    vertexBuffer.indices = indices; 
    {
        VkDeviceSize bufferSize = sizeof(vertexBuffer.vertices[0]) * vertexBuffer.vertices.size();
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(rendererContent, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
        
        void* data;
        vkMapMemory(rendererContent.device, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertexBuffer.vertices.data(), (size_t) bufferSize);
        vkUnmapMemory(rendererContent.device, stagingBufferMemory);

        createBuffer(rendererContent, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer.vertexBuffer, vertexBuffer.vertexBufferMemory);


        copyBuffer(rendererContent, stagingBuffer, vertexBuffer.vertexBuffer, bufferSize);

        vkDestroyBuffer(rendererContent.device, stagingBuffer, nullptr);
        vkFreeMemory(rendererContent.device, stagingBufferMemory, nullptr);
    }
    {
        VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(rendererContent, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(rendererContent.device, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, indices.data(), (size_t) bufferSize);
        vkUnmapMemory(rendererContent.device, stagingBufferMemory);

        createBuffer(rendererContent, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer.indexBuffer, vertexBuffer.indexBufferMemory);

        copyBuffer(rendererContent, stagingBuffer, vertexBuffer.indexBuffer, bufferSize);
        vkDestroyBuffer(rendererContent.device, stagingBuffer, nullptr);
        vkFreeMemory(rendererContent.device, stagingBufferMemory, nullptr);
    }
    return vertexBuffer; 

}
void freeVertexBuffer(RendererContent& rendererContent, VertexBuffer& vertexBuffer) {
    vkDestroyBuffer(rendererContent.device, vertexBuffer.indexBuffer, nullptr);
    vkFreeMemory(rendererContent.device, vertexBuffer.indexBufferMemory, nullptr);

    vkDestroyBuffer(rendererContent.device, vertexBuffer.vertexBuffer, nullptr);
    vkFreeMemory(rendererContent.device, vertexBuffer.vertexBufferMemory, nullptr);
}