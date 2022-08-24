#ifndef ENGINEPROTOTYPE_IMAGE_BLENDER
#define ENGINEPROTOTYPE_IMAGE_BLENDER

#include "renderer.h"
#include <vulkan/vulkan.h> 
#include <vulkan/vulkan_core.h>
#include "shader.h"

#define IMAGE_BLENDER_SHADER_PATH "assets/shaders/compiled/image_blender.comp.spv"

extern VkDescriptorSetLayout imageBlenderDescriptorSetLayout;
extern VkPipeline imageBlenderPipeline;
extern Shader imageBlenderShader;
extern VkPipelineLayout imageBlenderPipelineLayout;
extern VkDescriptorPool imageBlenderDescriptorPool;
extern VkDescriptorSet imageBlenderDescriptorSet;

void setupImageBlender(Renderer& renderer, uint32_t currentImageIndex, RenderPassResources& backgroundRenderPassResources, RenderPassResources& foregroundRenderPassResources);
void executeImageBlender(Renderer& renderer);

#endif
