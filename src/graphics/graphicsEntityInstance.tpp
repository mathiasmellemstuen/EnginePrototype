#include "graphicsEntityInstance.h"
#include <imgui/imgui.h>
#include "uniformBuffer.h"
#include "descriptorPool.h"

template<typename T> void GraphicsEntityInstance<T>::render(Renderer& renderer, int currentCommandBufferIndex) {
	

    VkCommandBuffer& commandBuffer = renderer.commandBuffers[currentCommandBufferIndex];
    
    updateUniformBuffer(renderer, this->uniformBuffer, uniformBufferObject, currentCommandBufferIndex);
    
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->graphicsEntity->graphicsPipeline);
    
    VkBuffer vertexBuffers[] = {graphicsEntity->vertexBuffer->vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    
    vkCmdBindIndexBuffer(commandBuffer, graphicsEntity->vertexBuffer->indexBuffer, 0, VK_INDEX_TYPE_UINT32);
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->graphicsEntity->pipelineLayout, 0, 1, &this->descriptorPool.descriptorSets[currentCommandBufferIndex], 0, nullptr);
    
    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(graphicsEntity->vertexBuffer->indices.size()), 1, 0, 0, 0);
}
template<typename T> void GraphicsEntityInstance<T>::reCreateGraphics(Renderer& renderer) {
    freeUniformBuffer(renderer, this->uniformBuffer); 
    freeDescriptorPool(renderer, this->descriptorPool);
    this->uniformBuffer = createUniformBuffer<T>(renderer);
    this->descriptorPool = createDescriptorPool<T>(renderer, *this);
}
