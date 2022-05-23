#include "descriptorSetLayout.h"
#include <vulkan/vulkan.h>
#include "logicalDevice.h"
#include <stdexcept>
#include "../utility/debug.h"
#include "renderer.h"
#include "renderObject.h"
#include "layoutBinding.h"

void DescriptorSetLayout::create(const std::vector<LayoutBinding>& bindings) {

    Debug::log(INFO, "Starting setup of Descriptor set layout");

    std::vector<VkDescriptorSetLayoutBinding> lBindings;

    int bindingIndex = 0;

    for(const LayoutBinding& binding : bindings) {
        VkDescriptorSetLayoutBinding b;
        b.binding = bindingIndex;
        b.descriptorCount = 1;
        b.descriptorType = binding.type;
        b.stageFlags = binding.stage;
        b.pImmutableSamplers = nullptr;
        bindingIndex += 1;
        lBindings.push_back(b);
    }
    //VkDescriptorSetLayoutBinding uboLayoutBinding{};
    //uboLayoutBinding.binding = 0;
    //uboLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_UNIFORM_BUFFER;
    //uboLayoutBinding.descriptorCount = 1;
    //uboLayoutBinding.stageFlags = VK_SHADER_STAGE_VERTEX_BIT;
    //uboLayoutBinding.pImmutableSamplers = nullptr; // Optional

    //VkDescriptorSetLayoutBinding samplerLayoutBinding{};
    //samplerLayoutBinding.binding = 1;
    //samplerLayoutBinding.descriptorCount = 1;
    //samplerLayoutBinding.descriptorType = VK_DESCRIPTOR_TYPE_COMBINED_IMAGE_SAMPLER;
    //samplerLayoutBinding.pImmutableSamplers = nullptr;
    //samplerLayoutBinding.stageFlags = VK_SHADER_STAGE_FRAGMENT_BIT;

    VkDescriptorSetLayoutCreateInfo layoutInfo{};
    layoutInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
    layoutInfo.bindingCount = static_cast<uint32_t>(lBindings.size());
    layoutInfo.pBindings = lBindings.data();

    if (vkCreateDescriptorSetLayout(renderObject.renderer.logicalDevice.device, &layoutInfo, nullptr, &descriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }

    Debug::log(SUCCESS, "Descriptor set layout setup successfull!"); 
}

DescriptorSetLayout::DescriptorSetLayout(RenderObject& renderObject, const std::vector<LayoutBinding>& bindings) : renderObject(renderObject) {
    create(bindings);
}

void DescriptorSetLayout::clean() {
    vkDestroyDescriptorSetLayout(renderObject.renderer.logicalDevice.device, descriptorSetLayout, nullptr);
}
