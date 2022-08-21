#ifndef ENGINEPROTOTYPE_UNIFORMBUFFER_TPP
#define ENGINEPROTOTYPE_UNIFORMBUFFER_TPP

#include "uniformBuffer.h"
#include "../utility/logging.h"
#include "renderer.h"

template<typename T> const UniformBuffer& createUniformBuffer(Renderer& renderer) {
    UniformBuffer* uniformBuffer = new UniformBuffer; 
	
    logger(INFO, "Starting creation of uniform buffers.");
    VkDeviceSize bufferSize = sizeof(T);
    uniformBuffer->uniformBuffers.resize(renderer.swapChainImages.size());
    uniformBuffer->uniformBuffersMemory.resize(renderer.swapChainImages.size());

    for (size_t i = 0; i < renderer.swapChainImages.size(); i++) {
        createBuffer(renderer, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffer->uniformBuffers[i], uniformBuffer->uniformBuffersMemory[i]);
    }

    logger(SUCCESS, "Successfully created all buffers!"); 

    return *uniformBuffer;
}
template<typename T> void updateUniformBuffer(Renderer& renderer, UniformBuffer& uniformBuffer, T& bufferContent, int currentCommandBufferIndex) {
    void* data;
    vkMapMemory(renderer.device, uniformBuffer.uniformBuffersMemory[currentCommandBufferIndex], 0, sizeof(bufferContent), 0, &data);
    memcpy(data, &bufferContent, sizeof(bufferContent));
    vkUnmapMemory(renderer.device, uniformBuffer.uniformBuffersMemory[currentCommandBufferIndex]);
}

#endif
