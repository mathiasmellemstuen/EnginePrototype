#include "vertexBuffer.h"
#include <stdexcept>
#include <cstring>
#include "../utility/debug.h"
#include <iostream>
#include "commandPool.h"
#include "renderer.h"

VertexBuffer::VertexBuffer(Renderer& renderer, std::vector<Vertex>& vertices, std::vector<uint32_t>& indices) : renderer(renderer) {

    this->vertices = vertices;
    this->indices = indices;

    createVertexBuffer(vertices);
    createIndexBuffer(indices);
};

void VertexBuffer::createVertexBuffer(std::vector<Vertex>& vertices) {
    Debug::log(INFO, "Creating vertex buffer.");
    
    VkDeviceSize bufferSize = sizeof(this->vertices[0]) * this->vertices.size();
    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    renderer.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);
    
    void* data;
    vkMapMemory(renderer.logicalDevice.device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, this->vertices.data(), (size_t) bufferSize);
    vkUnmapMemory(renderer.logicalDevice.device, stagingBufferMemory);

    renderer.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_VERTEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, vertexBuffer, vertexBufferMemory);


    renderer.copyBuffer(stagingBuffer, vertexBuffer, bufferSize);

    vkDestroyBuffer(renderer.logicalDevice.device, stagingBuffer, nullptr);
    vkFreeMemory(renderer.logicalDevice.device, stagingBufferMemory, nullptr);

    Debug::log(SUCCESS, "Vertex buffer created!"); 
};

void VertexBuffer::createIndexBuffer(std::vector<uint32_t>& indices) {
    VkDeviceSize bufferSize = sizeof(indices[0]) * indices.size();

    VkBuffer stagingBuffer;
    VkDeviceMemory stagingBufferMemory;
    renderer.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_SRC_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, stagingBuffer, stagingBufferMemory);

    void* data;
    vkMapMemory(renderer.logicalDevice.device, stagingBufferMemory, 0, bufferSize, 0, &data);
    memcpy(data, indices.data(), (size_t) bufferSize);
    vkUnmapMemory(renderer.logicalDevice.device, stagingBufferMemory);

    renderer.createBuffer(bufferSize, VK_BUFFER_USAGE_TRANSFER_DST_BIT | VK_BUFFER_USAGE_INDEX_BUFFER_BIT, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT, indexBuffer, indexBufferMemory);

    renderer.copyBuffer(stagingBuffer, indexBuffer, bufferSize);
    vkDestroyBuffer(renderer.logicalDevice.device, stagingBuffer, nullptr);
    vkFreeMemory(renderer.logicalDevice.device, stagingBufferMemory, nullptr);
}

void VertexBuffer::cleanup() {
    
    vkDestroyBuffer(renderer.logicalDevice.device, indexBuffer, nullptr);
    vkFreeMemory(renderer.logicalDevice.device, indexBufferMemory, nullptr);

    vkDestroyBuffer(renderer.logicalDevice.device, vertexBuffer, nullptr);
    vkFreeMemory(renderer.logicalDevice.device, vertexBufferMemory, nullptr);

}
VertexBuffer::~VertexBuffer() {
    cleanup();
};
