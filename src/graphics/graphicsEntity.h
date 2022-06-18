#ifndef ENGINEPROTOTYPE_GRAPHICS_ENTITY
#define ENGINEPROTOTYPE_GRAPHICS_ENTITY

#include <vulkan/vulkan.h>
#include "vertexBuffer.h"
#include "texture.h"
#include "shader.h"
#include "layoutBinding.h"
#include <stdexcept>

struct GraphicsEntity {
    VertexBuffer* vertexBuffer;
    Texture* texture; 
    Shader* shader;
    VkDescriptorSetLayout descriptorSetLayout;
    VkPipeline graphicsPipeline;
    VkPipelineLayout pipelineLayout;
};

GraphicsEntity createGraphicsEntity(RendererContent& rendererContent, VertexBuffer* vertexBuffer, Texture* texture, Shader* shader);
void freeGraphicsEntity(RendererContent& rendererContent, GraphicsEntity& graphicsEntity);

#endif
