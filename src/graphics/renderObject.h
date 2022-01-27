#ifndef ENGINEPROTOTYPE_RENDEROBJECT
#define ENGINEPROTOTYPE_RENDEROBJECT

#include "texture.h"
#include "shader.h"
#include "graphicsPipeline.h"
#include "vertexBuffer.h"
#include "uniformBuffer.h"
#include "descriptorSetLayout.h"
#include "descriptorPool.h"
#include "../core/transform.h"

#include <vector>
#include <glm/glm.hpp>

class Renderer;

struct RenderObject {
        Transform& transform;
        Renderer& renderer;
        Texture& texture;
        Shader shader;
        VertexBuffer vertexBuffer;
        DescriptorSetLayout descriptorSetLayout;
        DescriptorPool descriptorPool;
        GraphicsPipeline graphicsPipeline;
        std::function<void(RenderObject& self, VkCommandBuffer& commandBuffer, int currentCommandBuffer, uint32_t currentImage)> render;
};

#endif
