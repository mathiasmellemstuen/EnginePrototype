#include "imageBlender.h"
#include <vulkan/vulkan.h>
#include <vulkan/vulkan_core.h>
#include "../utility/logging.h"
#include <array>
#include "texture.h"

VkDescriptorSetLayout imageBlenderDescriptorSetLayout;

VkPipeline imageBlenderPipeline;
Shader imageBlenderShader;
VkPipelineLayout imageBlenderPipelineLayout;
VkDescriptorPool imageBlenderDescriptorPool;
VkDescriptorSet imageBlenderDescriptorSet;

void setupImageBlender(Renderer& renderer, uint32_t currentImageIndex, RenderPassResources& backgroundRenderPassResources, RenderPassResources& foregroundRenderPassResources) {

	// Creating textures
	Texture* backgroundTexture = new Texture;
	backgroundTexture->textureImage = backgroundRenderPassResources.attachments[currentImageIndex].image;
	backgroundTexture->textureImageView = backgroundRenderPassResources.attachments[currentImageIndex].view;
	createTextureSampler(renderer, *backgroundTexture);
	
	Texture* foregroundTexture = new Texture;
	foregroundTexture->textureImage = foregroundRenderPassResources.attachments[currentImageIndex].image;
	foregroundTexture->textureImageView = foregroundRenderPassResources.attachments[currentImageIndex].view;
	createTextureSampler(renderer, *foregroundTexture);

	// Create descirptors

	VkDescriptorSetLayoutBinding bindings[2];
	bindings[0].binding = 0;
	bindings[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
	bindings[0].descriptorCount = 1; 
	bindings[0].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;
	bindings[1].binding = 1;
	bindings[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
	bindings[1].descriptorCount = 1; 
	bindings[1].stageFlags = VK_SHADER_STAGE_COMPUTE_BIT;

	VkDescriptorSetLayoutCreateInfo layoutCreateInfo{};
	layoutCreateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO;
	layoutCreateInfo.bindingCount = static_cast<uint32_t>(2);
	layoutCreateInfo.pBindings = bindings; 

    if (vkCreateDescriptorSetLayout(renderer.device, &layoutCreateInfo, nullptr, &imageBlenderDescriptorSetLayout) != VK_SUCCESS) {
        throw std::runtime_error("failed to create descriptor set layout!");
    }

	imageBlenderShader = createShader(renderer, "", "", "", IMAGE_BLENDER_SHADER_PATH);
	
	VkPipelineLayoutCreateInfo pipelineLayoutCreateInfo{};
	pipelineLayoutCreateInfo.sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO;
	pipelineLayoutCreateInfo.setLayoutCount = 1; 
	pipelineLayoutCreateInfo.pSetLayouts = &imageBlenderDescriptorSetLayout;

    if (vkCreatePipelineLayout(renderer.device, &pipelineLayoutCreateInfo, nullptr, &imageBlenderPipelineLayout) != VK_SUCCESS) { 
        logger(ERROR, "Failed to create pipeline layout!"); 
        throw std::runtime_error("failed to create pipeline layout!"); 
    }

	logger(SUCCESS, "Created pipeline layout.");

	VkComputePipelineCreateInfo pipelineCreateInfo{};
	pipelineCreateInfo.sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO;
	pipelineCreateInfo.stage = imageBlenderShader.shaderStages[3];
	pipelineCreateInfo.layout = imageBlenderPipelineLayout; 

	if(vkCreateComputePipelines(renderer.device, VK_NULL_HANDLE, 1, &pipelineCreateInfo, nullptr, &imageBlenderPipeline) != VK_SUCCESS) {
		logger(ERROR, "Could not create compute pipeline"); 
		throw std::runtime_error("Could not create compute pipeline."); 
	}

	logger(SUCCESS, "Created compute pipeline."); 

    std::array<VkDescriptorPoolSize, 2> poolSizes{};
    poolSizes[0].type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    poolSizes[0].descriptorCount = static_cast<uint32_t>(renderer.swapChainImages.size());
    poolSizes[1].type = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
    poolSizes[1].descriptorCount = static_cast<uint32_t>(renderer.swapChainImages.size());

    VkDescriptorPoolCreateInfo poolInfo{};
    poolInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO;
    poolInfo.poolSizeCount = static_cast<uint32_t>(poolSizes.size());
    poolInfo.pPoolSizes = poolSizes.data();
    poolInfo.maxSets = static_cast<uint32_t>(renderer.swapChainImages.size());

    if (vkCreateDescriptorPool(renderer.device, &poolInfo, nullptr, &imageBlenderDescriptorPool) != VK_SUCCESS) {

        throw std::runtime_error("failed to create descriptor pool!");
    }

	logger(SUCCESS, "Created descriptor pool!");


    VkDescriptorSetAllocateInfo descriptorSetAllocateInfo{};
    descriptorSetAllocateInfo.sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO;
    descriptorSetAllocateInfo.descriptorPool = imageBlenderDescriptorPool;
    descriptorSetAllocateInfo.descriptorSetCount = static_cast<uint32_t>(1);
    descriptorSetAllocateInfo.pSetLayouts = &imageBlenderDescriptorSetLayout;
    
    if (vkAllocateDescriptorSets(renderer.device, &descriptorSetAllocateInfo, &imageBlenderDescriptorSet) != VK_SUCCESS) {
        throw std::runtime_error("failed to allocate descriptor sets!");
    }

	logger(SUCCESS, "Allocated descriptor sets!"); 

	std::array<VkWriteDescriptorSet, 2> descriptorWrites{};
	
	VkDescriptorImageInfo backgroundImageInfo{};
	backgroundImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	backgroundImageInfo.imageView = backgroundTexture->textureImageView;
	backgroundImageInfo.sampler = backgroundTexture->textureSampler;
	
	VkDescriptorImageInfo foregroundImageInfo{};
	backgroundImageInfo.imageLayout = VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
	backgroundImageInfo.imageView = foregroundTexture->textureImageView;
	backgroundImageInfo.sampler = foregroundTexture->textureSampler;
	
	descriptorWrites[0].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[0].dstSet = imageBlenderDescriptorSet;
	descriptorWrites[0].dstBinding = 1;
	descriptorWrites[0].dstArrayElement = 0;
	descriptorWrites[0].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
	descriptorWrites[0].descriptorCount = 1;
	descriptorWrites[0].pImageInfo = &backgroundImageInfo;

	descriptorWrites[1].sType = VK_STRUCTURE_TYPE_WRITE_DESCRIPTOR_SET;
	descriptorWrites[1].dstSet = imageBlenderDescriptorSet;
	descriptorWrites[1].dstBinding = 1;
	descriptorWrites[1].dstArrayElement = 0;
	descriptorWrites[1].descriptorType = VK_DESCRIPTOR_TYPE_STORAGE_IMAGE;
	descriptorWrites[1].descriptorCount = 1;
	descriptorWrites[1].pImageInfo = &foregroundImageInfo;
	
	vkUpdateDescriptorSets(renderer.device, static_cast<uint32_t>(2), descriptorWrites.data(), 0, nullptr);

	logger(SUCCESS, "Updated descriptor sets!"); 
}
