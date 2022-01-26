#include "renderObject.h"
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

RenderObject::RenderObject(Renderer& renderer, Texture& texture, Shader& shader, VertexBuffer& vertexBuffer) : renderer(renderer), texture(texture), shader(shader), vertexBuffer(vertexBuffer), descriptorSetLayout(*this), descriptorPool(*this), graphicsPipeline(*this) {

};

RenderObject::~RenderObject() {

};

void RenderObject::create() {

};

glm::mat4 RenderObject::createModelMatrix() {

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::rotate(model, 3.1415f / 4, glm::vec3(0.0f, 0.0f, 1.0f)); 
    model = glm::translate(model, position);
    model = glm::rotate(model, nextRotationAngles, glm::vec3(0.0f, 0.0f, 1.0f)); 

    return model; 
};
