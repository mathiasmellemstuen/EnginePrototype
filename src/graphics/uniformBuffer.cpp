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


void freeUniformBuffer(RendererContent& rendererContent, UniformBuffer& uniformBuffer) {
    for (size_t i = 0; i < rendererContent.swapChainImages.size(); i++) {
        vkDestroyBuffer(rendererContent.device, uniformBuffer.uniformBuffers[i], nullptr);
        vkFreeMemory(rendererContent.device, uniformBuffer.uniformBuffersMemory[i], nullptr);
    }
}