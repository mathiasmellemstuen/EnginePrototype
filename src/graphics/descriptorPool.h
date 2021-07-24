#ifndef ENGINEPROTOTYPE_DESCRIPTORPOOL
#define ENGINEPROTOTYPE_DESCRIPTORPOOL

#include <vulkan/vulkan.h>
#include "logicalDevice.h"
#include "swapChain.h"
#include <vector>
#include "uniformBuffer.h"
#include "descriptorSetLayout.h"

class DescriptorPool {
    public:
        VkDescriptorPool descriptorPool;
        std::vector<VkDescriptorSet> descriptorSets;
        VkDevice* device; 
        void create(LogicalDevice& logicalDevice, SwapChain& swapChain, UniformBuffer& uniformBuffer, DescriptorSetLayout& descriptorSetLayout);
        void createDescriptorSets(SwapChain& swapChain, UniformBuffer& uniformBuffer, DescriptorSetLayout& descriptorSetLayout);
        DescriptorPool(LogicalDevice& logicalDevice, SwapChain& swapChain, UniformBuffer& uniformBuffer, DescriptorSetLayout& descriptorSetLayout); 
        ~DescriptorPool(); 
};

#endif