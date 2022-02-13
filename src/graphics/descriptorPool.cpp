#include "descriptorPool.h"
#include <stdexcept>
#include "uniformBufferObject.h"
#include <array>
#include "texture.h"
#include "renderer.h"
#include "renderObject.h"
#include "../utility/debug.h"
#include <vector>

void DescriptorPool::create() {

    if(&renderObject == nullptr)
        return;

    Debug::log(INFO, "Creating descriptor pool");
    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(renderObject.renderer.swapChain.swapChainImages.size());
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = static_cast<uint32_t>(renderObject.renderer.swapChain.swapChainImages.size());

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(renderObject.renderer.swapChain.swapChainImages.size());

    if (vkCreateDescriptorPool(renderObject.renderer.logicalDevice.device, &poolInfo, nullptr, &descriptorPool) != VK_SUCCESS) {

        throw std::runtime_error("failed to create descriptor pool!");
    }

    Debug::log(SUCCESS, "Successfully created descriptor pool!"); 
    createDescriptorSets(); 
}

void DescriptorPool::createDescriptorSets() {
    Debug::log(INFO, "Creating descriptor sets"); 

    std::vector<VkDescriptorSetLayout> layouts(renderObject.renderer.swapChain.swapChainImages.size(), renderObject.descriptorSetLayout.descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(renderObject.renderer.swapChain.swapChainImages.size());
    allocInfo.pSetLayouts = layouts.data();

    descriptorSets.resize(renderObject.renderer.swapChain.swapChainImages.size());

    if (vkAllocateDescriptorSets(renderObject.renderer.logicalDevice.device, &allocInfo, descriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    for (size_t i = 0; i < renderObject.renderer.swapChain.swapChainImages.size(); i++) {

            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = renderObject.renderer.uniformBuffer.uniformBuffers[i];
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(UniformBufferObject);

            VkDescriptorImageInfo imageInfo{};
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView = renderObject.texture.textureImageView;//TODO: Support more than one texture here...
            imageInfo.sampler = renderObject.texture.textureSampler; // Here too...

            std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

            descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[0].dstSet = descriptorSets[i];
            descriptorWrites[0].dstBinding = 0;
            descriptorWrites[0].dstArrayElement = 0;
            descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrites[0].descriptorCount = 1;
            descriptorWrites[0].pBufferInfo = &bufferInfo;

            descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[1].dstSet = descriptorSets[i];
            descriptorWrites[1].dstBinding = 1;
            descriptorWrites[1].dstArrayElement = 0;
            descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptorWrites[1].descriptorCount = 1;
            descriptorWrites[1].pImageInfo = &imageInfo;

            vkUpdateDescriptorSets(renderObject.renderer.logicalDevice.device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);;

    }
    Debug::log(SUCCESS, "Created descriptor sets!");
}

DescriptorPool::DescriptorPool(RenderObject& renderObject) : renderObject(renderObject) {
    create(); 
}

void DescriptorPool::clean() {
    vkDestroyDescriptorPool(renderObject.renderer.logicalDevice.device, descriptorPool, nullptr);
}; 
