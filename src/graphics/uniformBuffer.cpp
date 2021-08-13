#include "uniformBuffer.h"
#include "logicalDevice.h"
#include "uniformBufferObject.h"
#include "physicalDevice.h"
#include "../utility/debug.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>
#include <cstring>

#include "renderer.h"

void UniformBuffer::update(uint32_t currentImage, glm::mat4& model) {
    
    static auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    UniformBufferObject ubo{};
    //ubo.model = model; 
    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(30.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f), 1920.0f / 1080.0f, 0.1f,10.0f);
    ubo.proj[1][1] *= -1;

    void* data;
    vkMapMemory(renderer.logicalDevice.device, uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(renderer.logicalDevice.device, uniformBuffersMemory[currentImage]);
}

void UniformBuffer::create() {
    
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);
    uniformBuffers.resize(renderer.swapChain.swapChainImages.size());
    uniformBuffersMemory.resize(renderer.swapChain.swapChainImages.size());

    allocatedSwapChainSize = renderer.swapChain.swapChainImages.size();

    for (size_t i = 0; i < allocatedSwapChainSize; i++) {
        renderer.createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
    }
}

UniformBuffer::UniformBuffer(Renderer& renderer) : renderer(renderer) {
    create(); 
}

UniformBuffer::~UniformBuffer() {
    for (size_t i = 0; i < allocatedSwapChainSize; i++) {
        vkDestroyBuffer(renderer.logicalDevice.device, uniformBuffers[i], nullptr);
        vkFreeMemory(renderer.logicalDevice.device, uniformBuffersMemory[i], nullptr);
    }
}