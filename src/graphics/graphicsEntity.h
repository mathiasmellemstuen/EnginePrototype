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
    RenderPassObject* renderPassObject;
    VkDescriptorSetLayout descriptorSetLayout;
    VkPipeline graphicsPipeline;
    VkPipelineLayout pipelineLayout;
    bool depthTestEnabled; 
};
extern std::vector<GraphicsEntity*> allGraphicsEntities;

GraphicsEntity& createGraphicsEntity(Renderer& renderer, RenderPassObject* renderPassObject, Shader* shader, VertexBuffer* vertexBuffer = nullptr, Texture* texture = nullptr, bool enableDepthTest = true, unsigned int pushConstantsSize = 0);
void freeGraphicsEntity(Renderer& renderer, GraphicsEntity& graphicsEntity);
void reCreateGraphicsEntity(Renderer& renderer, GraphicsEntity& graphicsEntity);

#endif
