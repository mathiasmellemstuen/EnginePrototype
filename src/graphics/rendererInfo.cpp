#include "rendererInfo.h"
#include <vulkan/vulkan.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

RendererInfo::RendererInfo(Renderer& renderer, Texture& texture, Shader& shader, VertexBuffer& vertexBuffer) : renderer(renderer), texture(texture), shader(shader), vertexBuffer(vertexBuffer), descriptorSetLayout(*this), descriptorPool(*this), graphicsPipeline(*this) {

};

RendererInfo::~RendererInfo() {

};

void RendererInfo::create() {

};

glm::mat4 RendererInfo::createModelMatrix() {

    glm::mat4 model = glm::mat4(1.0f);
    model = glm::translate(model, position);
    model = glm::rotate(model, nextRotationAngles, nextRotationDirection);

    return model; 
};