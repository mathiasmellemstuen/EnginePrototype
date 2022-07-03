#ifndef ENGINEPROTOTYPE_UNIFORMBUFFER
#define ENGINEPROTOTYPE_UNIFORMBUFFER

#include <vector>
#include <vulkan/vulkan.h>
#include <glm/vec3.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "renderer.h"

struct UniformBuffer {
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;
};

template<typename T> const UniformBuffer& createUniformBuffer(Renderer& renderer);
template<typename T> void updateUniformBuffer(Renderer& renderer, UniformBuffer& uniformBuffer, T& bufferContent, int currentCommandBufferIndex);
void freeUniformBuffer(Renderer& renderer, UniformBuffer& uniformBuffer);

#include "uniformBuffer.tpp"
#endif