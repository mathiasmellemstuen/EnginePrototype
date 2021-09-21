#ifndef ENGINEPROTOTYPE_RENDEROBJECT
#define ENGINEPROTOTYPE_RENDEROBJECT

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

class RenderObject {
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
        
        RenderObject(Renderer& renderer, Texture& texture, Shader& shader, VertexBuffer& vertexBuffer);
        ~RenderObject();
        void create();
        glm::mat4 createModelMatrix();
};

#endif
