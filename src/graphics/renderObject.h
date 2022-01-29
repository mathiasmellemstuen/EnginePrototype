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
#include <functional>

#include "renderer.h"
#include "../core/component.h"

class RenderObject {
    public: 
        Renderer& renderer;
        Texture& texture;
        Shader& shader;
        VertexBuffer& vertexBuffer;
        DescriptorSetLayout descriptorSetLayout;
        DescriptorPool descriptorPool;
        GraphicsPipeline graphicsPipeline;
        std::function<void(Transform& transform, VkCommandBuffer& commandBuffer, int currentCommandBuffer, uint32_t currentImage)> render;

        RenderObject(Renderer& renderer, Texture& texture, Shader& shader, VertexBuffer& vertexBuffer);
        
        void setDefaultRendering();
};

#endif
