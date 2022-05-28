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

UniformBuffer createUniformBuffer(RendererContent& rendererContent);
void updateUniformBuffer(RendererContent& rendererContent, UniformBuffer& uniformBuffer, glm::mat4& view, glm::mat4& projection, glm::mat4& model);
void freeUniformBuffer(RendererContent& rendererContent, UniformBuffer& uniformBuffer);
#endif