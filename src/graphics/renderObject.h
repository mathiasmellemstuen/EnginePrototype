#ifndef ENGINEPROTOTYPE_RENDEROBJECT
#define ENGINEPROTOTYPE_RENDEROBJECT

#include "texture.h"
#include "shader.h"
#include "graphicsPipeline.h"
#include "vertexBuffer.h"
#include "descriptorSetLayout.h"
#include "../core/transform.h"

#include <vector>
#include <glm/glm.hpp>
#include <functional>

#include "../core/component.h"

#include "renderInstance.h"

class Camera;

class RenderObject {
    public: 
        Renderer& renderer;
        Texture& texture;
        Shader& shader;
        VertexBuffer& vertexBuffer;
        DescriptorSetLayout descriptorSetLayout;
        GraphicsPipeline graphicsPipeline;
        std::function<void(Camera& camera, RenderInstance& renderInstance, VkCommandBuffer& commandBuffer, int currentCommandBuffer, uint32_t currentImage)> render;

        RenderObject(Renderer& renderer, Texture& texture, Shader& shader, VertexBuffer& vertexBuffer);
        
        void setDefaultRendering();
};

#endif
