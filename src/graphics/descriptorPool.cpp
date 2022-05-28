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

DescriptorPool createDescriptorPool(RendererContent& rendererContent, GraphicsEntityInstance& graphicsEntityInstance) {
    DescriptorPool descriptorPool;
    
    Debug::log(INFO, "Creating descriptor pool");
    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(rendererContent.swapChainImages.size());
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    poolSizes[1].descriptorCount = static_cast<uint32_t>(rendererContent.swapChainImages.size());

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(rendererContent.swapChainImages.size());

    if (vkCreateDescriptorPool(rendererContent.device, &poolInfo, nullptr, &descriptorPool.descriptorPool) != VK_SUCCESS) {

        throw std::runtime_error("failed to create descriptor pool!");
    }

    Debug::log(SUCCESS, "Successfully created descriptor pool!"); 
    Debug::log(INFO, "Creating descriptor sets"); 

    std::vector<VkDescriptorSetLayout> layouts(rendererContent.swapChainImages.size(), graphicsEntityInstance.graphicsEntity->descriptorSetLayout);
    VkDescriptorSetAllocateInfo allocInfo{};
    allocInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    allocInfo.descriptorPool = descriptorPool.descriptorPool;
    allocInfo.descriptorSetCount = static_cast<uint32_t>(rendererContent.swapChainImages.size());
    allocInfo.pSetLayouts = layouts.data();

    descriptorPool.descriptorSets.resize(rendererContent.swapChainImages.size());
    
    if (vkAllocateDescriptorSets(rendererContent.device, &allocInfo, descriptorPool.descriptorSets.data()) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

    for (size_t i = 0; i < rendererContent.swapChainImages.size(); i++) {

            VkDescriptorBufferInfo bufferInfo{};
            bufferInfo.buffer = graphicsEntityInstance.uniformBuffer.uniformBuffers[i];
            bufferInfo.offset = 0;
            bufferInfo.range = sizeof(UniformBufferObject);

            VkDescriptorImageInfo imageInfo{};
            imageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
            imageInfo.imageView = graphicsEntityInstance.graphicsEntity->texture->textureImageView;//TODO: Support more than one texture here...
            imageInfo.sampler = graphicsEntityInstance.graphicsEntity->texture->textureSampler; // Here too...

            std::array<VkWriteDescriptorSet, 2> descriptorWrites{};

            descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[0].dstSet = descriptorPool.descriptorSets[i];
            descriptorWrites[0].dstBinding = 0;
            descriptorWrites[0].dstArrayElement = 0;
            descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
            descriptorWrites[0].descriptorCount = 1;
            descriptorWrites[0].pBufferInfo = &bufferInfo;

            descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
            descriptorWrites[1].dstSet = descriptorPool.descriptorSets[i];
            descriptorWrites[1].dstBinding = 1;
            descriptorWrites[1].dstArrayElement = 0;
            descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
            descriptorWrites[1].descriptorCount = 1;
            descriptorWrites[1].pImageInfo = &imageInfo;

            vkUpdateDescriptorSets(rendererContent.device, static_cast<uint32_t>(descriptorWrites.size()), descriptorWrites.data(), 0, nullptr);;

    }
    Debug::log(SUCCESS, "Created descriptor sets!");


    return descriptorPool;
}
void freeDescriptorPool(RendererContent& rendererContent, DescriptorPool& descriptorPool) {
    vkDestroyDescriptorPool(rendererContent.device, descriptorPool.descriptorPool, nullptr);
}