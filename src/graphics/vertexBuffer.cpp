#include "vertexBuffer.h"
#include "renderer.h"
#include "../utility/logging.h"

const VertexBuffer& createVertexBuffer(Renderer& renderer, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices) {
    VertexBuffer* vertexBuffer = new VertexBuffer; 
    vertexBuffer->vertices = vertices; 
    vertexBuffer->indices = indices; 
    {
        VkDeviceSize bufferSize = sizeof(vertexBuffer->vertices[0]) * vertexBuffer->vertices.size();
        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(renderer, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
        
        void* data;
        vkMapMemory(renderer.device, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, vertexBuffer->vertices.data(), (size_t) bufferSize);
        vkUnmapMemory(renderer.device, stagingBufferMemory);

        createBuffer(renderer, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer->vertexBuffer, vertexBuffer->vertexBufferMemory);


        copyBuffer(renderer, stagingBuffer, vertexBuffer->vertexBuffer, bufferSize);

        vkDestroyBuffer(renderer.device, stagingBuffer, nullptr);
        vkFreeMemory(renderer.device, stagingBufferMemory, nullptr);
    }
    {
        VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

        VkBuffer stagingBuffer;
        VkDeviceMemory stagingBufferMemory;
        createBuffer(renderer, bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

        void* data;
        vkMapMemory(renderer.device, stagingBufferMemory, 0, bufferSize, 0, &data);
        memcpy(data, indices.data(), (size_t) bufferSize);
        vkUnmapMemory(renderer.device, stagingBufferMemory);

        createBuffer(renderer, bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer->indexBuffer, vertexBuffer->indexBufferMemory);

        copyBuffer(renderer, stagingBuffer, vertexBuffer->indexBuffer, bufferSize);
        vkDestroyBuffer(renderer.device, stagingBuffer, nullptr);
        vkFreeMemory(renderer.device, stagingBufferMemory, nullptr);
    }
    return *vertexBuffer; 

}
void freeVertexBuffer(Renderer& renderer, VertexBuffer& vertexBuffer) {
    vkDestroyBuffer(renderer.device, vertexBuffer.indexBuffer, nullptr);
    vkFreeMemory(renderer.device, vertexBuffer.indexBufferMemory, nullptr);

    vkDestroyBuffer(renderer.device, vertexBuffer.vertexBuffer, nullptr);
    vkFreeMemory(renderer.device, vertexBuffer.vertexBufferMemory, nullptr);

    delete &vertexBuffer;
}