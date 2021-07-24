#include "descriptorSetLayout.h"
#include <vulkan/vulkan.h>
#include "logicalDevice.h"
#include <stdexcept>
#include "../utility/log.h"

void DescriptorSetLayout::create(LogicalDevice& logicalDevice) {

    log(INFO, "Starting setup of Descriptor set layout"); 
    VkDescriptorSetLayoutBinding uboLayoutBinding{};
    uboLayoutBinding.binding = 0;
    uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    uboLayoutBinding.descriptorCount = 1;
    uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = 1;
    layoutInfo.pBindings = &uboLayoutBinding;

    if (vkCreateDescriptorSetLayout(*device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }

    log(SUCCESS, "Descriptor set layout setup successfull!"); 
}

DescriptorSetLayout::DescriptorSetLayout(LogicalDevice& logicalDevice) {
    this->device = &logicalDevice.device; 
    create(logicalDevice);
}

DescriptorSetLayout::~DescriptorSetLayout() {
    vkDestroyDescriptorSetLayout(*device, descriptorSetLayout, nullptr);
}