#ifndef ENGINEPROTOTYPE_DESCRIPTORPOOL
#define ENGINEPROTOTYPE_DESCRIPTORPOOL

#include <vulkan/vulkan.h>
#include "logicalDevice.h"
#include "swapChain.h"
#include <vector>
#include "uniformBuffer.h"
#include "descriptorSetLayout.h"
#include "texture.h"

class DescriptorPool {
    public:
        VkDescriptorPool descriptorPool;
        std::vector<VkDescriptorSet> descriptorSets;
        VkDevice* device; 
        void create(LogicalDevice& logicalDevice, SwapChain& swapChain, UniformBuffer& uniformBuffer, DescriptorSetLayout& descriptorSetLayout, Texture& texture);
        void createDescriptorSets(SwapChain& swapChain, UniformBuffer& uniformBuffer, DescriptorSetLayout& descriptorSetLayout, Texture& texture);
        DescriptorPool(LogicalDevice& logicalDevice, SwapChain& swapChain, UniformBuffer& uniformBuffer, DescriptorSetLayout& descriptorSetLayout, Texture& texture); 
        ~DescriptorPool(); 
};

#endif