#ifndef ENGINEPROTOTYPE_UNIFORMBUFFER_TPP
#define ENGINEPROTOTYPE_UNIFORMBUFFER_TPP

#include "uniformBuffer.h"
#include "../utility/logging.h"
#include "renderer.h"

template<typename T> UniformBuffer createUniformBuffer(RendererContent& rendererContent) {
    UniformBuffer uniformBuffer; 
    
    logger(INFO, "Starting creation of uniform buffers.");
    VkDeviceSize bufferSize = sizeof(T);
    uniformBuffer.uniformBuffers.resize(rendererContent.swapChainImages.size());
    uniformBuffer.uniformBuffersMemory.resize(rendererContent.swapChainImages.size());

    for (size_t i = 0; i < rendererContent.swapChainImages.size(); i++) {
        createBuffer(rendererContent, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffer.uniformBuffers[i], uniformBuffer.uniformBuffersMemory[i]);
    }

    logger(SUCCESS, "Successfully created all buffers!"); 

    return uniformBuffer;
}
template<typename T> void updateUniformBuffer(RendererContent& rendererContent, UniformBuffer& uniformBuffer, T& bufferContent, int currentCommandBufferIndex) {
    void* data;
    vkMapMemory(rendererContent.device, uniformBuffer.uniformBuffersMemory[currentCommandBufferIndex], 0, sizeof(bufferContent), 0, &data);
    memcpy(data, &bufferContent, sizeof(bufferContent));
    vkUnmapMemory(rendererContent.device, uniformBuffer.uniformBuffersMemory[currentCommandBufferIndex]);
}

#endif
