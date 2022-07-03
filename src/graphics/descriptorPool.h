#ifndef ENGINEPROTOTYPE_DESCRIPTOR_POOL
#define ENGINEPROTOTYPE_DESCRIPTOR_POOL

#include <vulkan/vulkan.h>
#include <vector>
#include "renderer.h"
#include "../core/component.h"
#include <stdexcept>

template<typename U> class GraphicsEntityInstance;

struct DescriptorPool {
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;
};

template<typename T> const DescriptorPool& createDescriptorPool(Renderer& renderer, GraphicsEntityInstance<T>& graphicsEntityInstance);
template<typename T> void reCreateDescriptorPool(Renderer& renderer, DescriptorPool& descriptorPool, GraphicsEntityInstance<T>& graphicsEntityInstance);
void freeDescriptorPool(Renderer& renderer, DescriptorPool& descriptorPool);
#include "descriptorPool.tpp"

#endif