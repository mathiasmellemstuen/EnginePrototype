#include "descriptorPool.h"
#include <stdexcept>
#include "uniformBufferObject.h"
#include <array>
#include "texture.h"
#include "renderer.h"
#include "../utility/debug.h"
#include <vector>
#include <vulkan/vulkan.h>
#include "graphicsEntityInstance.h"

void freeDescriptorPool(Renderer& renderer, DescriptorPool& descriptorPool) {
    vkDestroyDescriptorPool(renderer.device, descriptorPool.descriptorPool, nullptr);
}