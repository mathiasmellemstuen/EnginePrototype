#include "renderObject.h"
#include "camera.h"
#include "../utility/debug.h"
#include <functional>
#include <iostream>

RenderObject::RenderObject(Renderer& renderer, Texture& texture, Shader& shader, VertexBuffer& vertexBuffer) : renderer(renderer), texture(texture), shader(shader), vertexBuffer(vertexBuffer), descriptorSetLayout(*this), descriptorPool(*this), graphicsPipeline(*this) {
    setDefaultRendering();
};

void RenderObject::setDefaultRendering() {
    render = [&](Camera& camera, Transform& transform, VkCommandBuffer& commandBuffer, int currentCommandBuffer, uint32_t currentImage) {

        renderer.uniformBuffer.update(currentImage, camera.view, camera.projection, transform.getModel());
        vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.graphicsPipeline);
        VkBuffer vertexBuffers[] = {vertexBuffer.vertexBuffer};
        VkDeviceSize offsets[] = {0};
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(commandBuffer, vertexBuffer.indexBuffer, 0, VK_INDEX_TYPE_UINT32);

        vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline.pipelineLayout, 0, 1, &descriptorPool.descriptorSets[currentCommandBuffer], 0, nullptr);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(vertexBuffer.indices.size()), 1, 0, 0, 0);
    };
};