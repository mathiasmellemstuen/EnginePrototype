#include "descriptorPool.h"
#include <stdexcept>
#include "uniformBufferObject.h"
void DescriptorPool::create(LogicalDevice& logicalDevice, SwapChain& swapChain, UniformBuffer& uniformBuffer, DescriptorSetLayout& descriptorSetLayout) {
    VkDescriptorPoolSize poolSize{};
    poolSize.type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSize.descriptorCount = static_cast<uint32_t>(swapChain.swapChainImages.size());

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = 1;
    poolInfo.pPoolSizes = &poolSize;

    poolInfo.maxSets = static_cast<uint32_t>(swapChain.swapChainImages.size());

    if (vkCreateDescriptorPool(*device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {

        throw std::runtime_error("failed to create descriptor pool!");
    }

    createDescriptorSets(swapChain, uniformBuffer, descriptorSetLayout); 
}

void DescriptorPool::createDescriptorSets(SwapChain& swapChain, UniformBuffer& uniformBuffer, DescriptorSetLayout& descriptorSetLayout) {
    std::vector<VkDescriptorSetLayout> layouts(swapChain.swapChainImages.size(), descriptorSetLayout.descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(swapChain.swapChainImages.size());
    allocInfo.pSetLayouts = layouts.data();

    descriptorSets.resize(swapChain.swapChainImages.size());

    if (vkAllocateDescriptorSets(*device, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    for (size_t i = 0; i < swapChain.swapChainImages.size(); i++) {

        VkDescriptorBufferInfo bufferInfo{};
        bufferInfo.buffer = uniformBuffer.uniformBuffers[i];
        bufferInfo.offset = 0;
        bufferInfo.range = sizeof(UniformBufferObject);

        VkWriteDescriptorSet descriptorWrite{};
        descriptorWrite.sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
        descriptorWrite.dstSet = descriptorSets[i];
        descriptorWrite.dstBinding = 0;
        descriptorWrite.dstArrayElement = 0;
        descriptorWrite.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
        descriptorWrite.descriptorCount = 1;
        descriptorWrite.pBufferInfo = &bufferInfo;
        descriptorWrite.pImageInfo = nullptr; // Optional
        descriptorWrite.pTexelBufferView = nullptr; // Optional
        vkUpdateDescriptorSets(*device, 1, &descriptorWrite, 0, nullptr);
    }
}

DescriptorPool::DescriptorPool(LogicalDevice& logicalDevice, SwapChain& swapChain, UniformBuffer& uniformBuffer, DescriptorSetLayout& descriptorSetLayout) {
    this->device = &logicalDevice.device;
    create(logicalDevice, swapChain, uniformBuffer, descriptorSetLayout); 
}

DescriptorPool::~DescriptorPool() {
    vkDestroyDescriptorPool(*device, descriptorPool, nullptr);
}