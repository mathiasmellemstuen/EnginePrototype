#include "graphicsEntityInstance.h"
#include <imgui/imgui.h>

template<typename T> void GraphicsEntityInstance<T>::debug() {
    ImGui::Text(std::string(name + " #" + std::to_string(id)).c_str());
}

template<typename T> void GraphicsEntityInstance<T>::render(RendererContent& rendererContent, int currentCommandBufferIndex) {

    VkCommandBuffer& commandBuffer = rendererContent.commandBuffers[currentCommandBufferIndex];
    
    updateUniformBuffer(rendererContent, this->uniformBuffer, uniformBufferObject, currentCommandBufferIndex);
    
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->graphicsEntity->graphicsPipeline);
    
    VkBuffer vertexBuffers[] = {graphicsEntity->vertexBuffer->vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    
    vkCmdBindIndexBuffer(commandBuffer, graphicsEntity->vertexBuffer->indexBuffer, 0, VK_INDEX_TYPE_UINT32);
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->graphicsEntity->pipelineLayout, 0, 1, &this->descriptorPool.descriptorSets[currentCommandBufferIndex], 0, nullptr);
    
    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(graphicsEntity->vertexBuffer->indices.size()), 1, 0, 0, 0);
}