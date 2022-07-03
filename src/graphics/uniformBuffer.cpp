#include "uniformBuffer.h"
#include "uniformBufferObject.h"
#include "../utility/logging.h"

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <chrono>
#include <cstring>

#include "../utility/debug.h"
#include "renderer.h"


void freeUniformBuffer(Renderer& renderer, UniformBuffer& uniformBuffer) {
    for (size_t i = 0; i < renderer.swapChainImages.size(); i++) {
        vkDestroyBuffer(renderer.device, uniformBuffer.uniformBuffers[i], nullptr);
        vkFreeMemory(renderer.device, uniformBuffer.uniformBuffersMemory[i], nullptr);
    }
}