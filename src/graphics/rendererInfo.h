#ifndef ENGINEPROTOTYPE_RENDERERINFO
#define ENGINEPROTOTYPE_RENDERERINFO

#include "texture.h"
#include "shader.h"
#include "graphicsPipeline.h"
#include "vertexBuffer.h"
#include "uniformBuffer.h"
#include "descriptorSetLayout.h"
#include "descriptorPool.h"

#include <vector>
#include <glm/glm.hpp>

class Renderer;

class RendererInfo {
    public:
        glm::vec3 position;
        glm::vec3 nextRotationDirection;
        float nextRotationAngles; 
        Renderer& renderer;
        Texture& texture;
        Shader shader;
        VertexBuffer vertexBuffer;
        DescriptorSetLayout descriptorSetLayout;
        DescriptorPool descriptorPool;
        GraphicsPipeline graphicsPipeline;
        
        RendererInfo(Renderer& renderer, Texture& texture, Shader& shader, VertexBuffer& vertexBuffer);
        ~RendererInfo();
        void create();
        glm::mat4 createModelMatrix();
};

#endif