#ifndef ENGINEPROTOTYPE_GRAPHICS_ENTITY
#define ENGINEPROTOTYPE_GRAPHICS_ENTITY

#include <vulkan/vulkan.h>
#include "vertexBuffer.h"
#include "texture.h"
#include "shader.h"
#include "layoutBinding.h"
#include <stdexcept>
#include <vector>

struct GraphicsEntity {
    VertexBuffer* vertexBuffer;
    Texture* texture; 
    Shader* shader;
    VkDescriptorSetLayout descriptorSetLayout;
    VkPipeline graphicsPipeline;
    VkPipelineLayout pipelineLayout;
    bool depthTestEnabled; 
};
extern std::vector<GraphicsEntity*> allGraphicsEntities;

const GraphicsEntity& createGraphicsEntity(RendererContent& rendererContent, Shader* shader, VertexBuffer* vertexBuffer = nullptr, Texture* texture = nullptr, bool enableDepthTest = true, unsigned int pushConstantsSize = 0);
void freeGraphicsEntity(RendererContent& rendererContent, GraphicsEntity& graphicsEntity);
void reCreateGraphicsEntity(RendererContent& rendererContent, GraphicsEntity& graphicsEntity);

#endif
