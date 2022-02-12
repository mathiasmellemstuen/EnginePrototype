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
#include "../utility/debug.h"

void UniformBuffer::update(uint32_t currentImage, glm::mat4& view, glm::mat4& projection, glm::mat4& model) {
    
    UniformBufferObject ubo{};
    ubo.model = model; 
    ubo.view = view;
    ubo.proj = projection;
    ubo.proj[1][1] *= -1;

    void* data;
    vkMapMemory(renderer.logicalDevice.device, uniformBuffersMemory[currentImage], 0, sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(renderer.logicalDevice.device, uniformBuffersMemory[currentImage]);
}

void UniformBuffer::create() {
    
    Debug::log(INFO, "Starting creation of uniform buffers."); 
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);
    uniformBuffers.resize(renderer.swapChain.swapChainImages.size());
    uniformBuffersMemory.resize(renderer.swapChain.swapChainImages.size());

    allocatedSwapChainSize = renderer.swapChain.swapChainImages.size();
    
    for (size_t i = 0; i < allocatedSwapChainSize; i++) {
        renderer.createBuffer(bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffers[i], uniformBuffersMemory[i]);
    }

    Debug::log(SUCCESS, "Successfully created all buffers!"); 
}

UniformBuffer::UniformBuffer(Renderer& renderer) : renderer(renderer) {
    create(); 
}

void UniformBuffer::clean() {
    for (size_t i = 0; i < allocatedSwapChainSize; i++) {
        vkDestroyBuffer(renderer.logicalDevice.device, uniformBuffers[i], nullptr);
        vkFreeMemory(renderer.logicalDevice.device, uniformBuffersMemory[i], nullptr);
    }
}
