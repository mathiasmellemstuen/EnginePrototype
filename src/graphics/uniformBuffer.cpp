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

#include "vulkanHelperFunctions.h"

void UniformBuffer::update(uint32_t currentImage) {
    
    static auto startTime = std::chrono::high_resolution_clock::now();
    auto currentTime = std::chrono::high_resolution_clock::now();
    float time = std::chrono::duration<float, std::chrono::seconds::period>(currentTime - startTime).count();

    UniformBufferObject ubo{};
    ubo.model = glm::rotate(glm::mat4(1.0f), time * glm::radians(90.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.view = glm::lookAt(glm::vec3(2.0f, 2.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 1.0f));
    ubo.proj = glm::perspective(glm::radians(45.0f), 800.0f / 600.0f, 0.1f,10.0f);
    ubo.proj[1][1] *= -1;

    void* data;
    vkMapMemory(*device, uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(*device, uniformBuffersMemory[currentImage]);
}

void UniformBuffer::create(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, SwapChain& swapChain) {
    
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);
    uniformBuffers.resize(swapChain.swapChainImages.size());
    uniformBuffersMemory.resize(swapChain.swapChainImages.size());

    allocatedSwapChainSize = swapChain.swapChainImages.size();

    for (size_t i = 0; i < allocatedSwapChainSize; i++) {
        createBuffer(physicalDevice, logicalDevice, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
    }
}

UniformBuffer::UniformBuffer(PhysicalDevice& physicalDevice, LogicalDevice& logicalDevice, SwapChain& swapChain) {
    this->device = &logicalDevice.device; 
    create(physicalDevice, logicalDevice, swapChain); 
}

UniformBuffer::~UniformBuffer() {
    for (size_t i = 0; i < allocatedSwapChainSize; i++) {
        vkDestroyBuffer(*device, uniformBuffers[i], nullptr);
        vkFreeMemory(*device, uniformBuffersMemory[i], nullptr);
    }
}