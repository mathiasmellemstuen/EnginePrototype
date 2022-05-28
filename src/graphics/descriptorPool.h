#ifndef ENGINEPROTOTYPE_DESCRIPTOR_POOL
#define ENGINEPROTOTYPE_DESCRIPTOR_POOL

#include <vulkan/vulkan.h>
#include <vector>
#include "renderer.h"

class GraphicsEntityInstance; 

struct DescriptorPool {
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;
};

DescriptorPool createDescriptorPool(RendererContent& rendererContent, GraphicsEntityInstance& graphicsEntityInstance);
void freeDescriptorPool(RendererContent& rendererContent, DescriptorPool& descriptorPool);

#endif