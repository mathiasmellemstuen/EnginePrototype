#ifndef ENGINEPROTOTYPE_DESCRIPTOR_POOL
#define ENGINEPROTOTYPE_DESCRIPTOR_POOL

#include <vulkan/vulkan.h>
#include <vector>
#include "renderer.h"
#include "../core/component.h"

template<typename U> class GraphicsEntityInstance;

struct DescriptorPool {
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;
};

template<typename T> DescriptorPool createDescriptorPool(RendererContent& rendererContent, GraphicsEntityInstance<T>& graphicsEntityInstance);
void freeDescriptorPool(RendererContent& rendererContent, DescriptorPool& descriptorPool);

#include "descriptorPool.tpp"

#endif