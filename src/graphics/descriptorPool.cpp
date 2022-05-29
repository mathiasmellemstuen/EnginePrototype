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

void freeDescriptorPool(RendererContent& rendererContent, DescriptorPool& descriptorPool) {
    vkDestroyDescriptorPool(rendererContent.device, descriptorPool.descriptorPool, nullptr);
}