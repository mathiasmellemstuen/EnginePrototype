#include "graphicsEntityInstance.h"
#include "../utility/debug.h"
#include "../core/component.h"
#include <imgui/imgui.h>
#include <vulkan/vulkan.h>
#include "uniformBuffer.h"
#include "descriptorPool.h"

GraphicsEntityInstance::GraphicsEntityInstance(RendererContent& rendererContent, GraphicsEntity* graphicsEntity) {
    this->graphicsEntity = graphicsEntity; 
    this->uniformBuffer = createUniformBuffer(rendererContent);
    this->descriptorPool = createDescriptorPool(rendererContent, *this);
}

void GraphicsEntityInstance::debug() {
    ImGui::Text(std::string(name + " #" + std::to_string(id)).c_str());
    ImGui::Text("Graphics Entity Instance");
}

void GraphicsEntityInstance::render(RendererContent& rendererContent, glm::mat4& view, glm::mat4& projection, glm::mat4& model, int currentCommandBufferIndex) {

    VkCommandBuffer& commandBuffer = rendererContent.commandBuffers[currentCommandBufferIndex];
    
    updateUniformBuffer(rendererContent, this->uniformBuffer, view, projection, model, currentCommandBufferIndex);
    
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->graphicsEntity->graphicsPipeline);
    
    VkBuffer vertexBuffers[] = {graphicsEntity->vertexBuffer->vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    
    vkCmdBindIndexBuffer(commandBuffer, graphicsEntity->vertexBuffer->indexBuffer, 0, VK_INDEX_TYPE_UINT32);
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->graphicsEntity->pipelineLayout, 0, 1, &this->descriptorPool.descriptorSets[currentCommandBufferIndex], 0, nullptr);
    
    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(graphicsEntity->vertexBuffer->indices.size()), 1, 0, 0, 0);
}