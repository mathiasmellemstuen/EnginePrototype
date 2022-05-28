#include "uniformBuffer.h"
#include "uniformBufferObject.h"
#include "../utility/debug.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>
#include <cstring>

#include "../utility/debug.h"
#include "renderer.h"

UniformBuffer createUniformBuffer(RendererContent& rendererContent) {
    UniformBuffer uniformBuffer; 
    
    Debug::log(INFO, "Starting creation of uniform buffers.");
    VkDeviceSize bufferSize = sizeof(UniformBufferObject);
    uniformBuffer.uniformBuffers.resize(rendererContent.swapChainImages.size());
    uniformBuffer.uniformBuffersMemory.resize(rendererContent.swapChainImages.size());

    for (size_t i = 0; i < rendererContent.swapChainImages.size(); i++) {
        createBuffer(rendererContent, bufferSize, VK_BUFFER_USAGE_UNIFORM_BUFFER_BIT, VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT, uniformBuffer.uniformBuffers[i], uniformBuffer.uniformBuffersMemory[i]);
    }

    Debug::log(SUCCESS, "Successfully created all buffers!"); 

    return uniformBuffer;
}
void updateUniformBuffer(RendererContent& rendererContent, UniformBuffer& uniformBuffer, glm::mat4& view, glm::mat4& projection, glm::mat4& model, int currentCommandBufferIndex) {
    UniformBufferObject ubo{};
    ubo.model = model; 
    ubo.view = view;
    ubo.proj = projection;

    //TODO: RendererContent.currentFrame might be the wrong value... Changed from current image
    void* data;
    vkMapMemory(rendererContent.device, uniformBuffer.uniformBuffersMemory[currentCommandBufferIndex], 0, sizeof(ubo), 0, &data);
    memcpy(data, &ubo, sizeof(ubo));
    vkUnmapMemory(rendererContent.device, uniformBuffer.uniformBuffersMemory[currentCommandBufferIndex]);
}

void freeUniformBuffer(RendererContent& rendererContent, UniformBuffer& uniformBuffer) {
    for (size_t i = 0; i < rendererContent.swapChainImages.size(); i++) {
        vkDestroyBuffer(rendererContent.device, uniformBuffer.uniformBuffers[i], nullptr);
        vkFreeMemory(rendererContent.device, uniformBuffer.uniformBuffersMemory[i], nullptr);
    }
}