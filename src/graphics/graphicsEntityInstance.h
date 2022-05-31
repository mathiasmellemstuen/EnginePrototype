#ifndef ENGINEPROTOTYPE_GRAPHICSENTITYINSTANCE
#define ENGINEPROTOTYPE_GRAPHICSENTITYINSTANCE

#include "uniformBuffer.h"
#include "graphicsEntity.h"
#include "renderer.h"
#include "../core/component.h"
#include "uniformBufferObject.h"
#include "descriptorPool.h"

template<typename T> class GraphicsEntityInstance : public Component {
    public:
        GraphicsEntity* graphicsEntity;
        UniformBuffer uniformBuffer;
        DescriptorPool descriptorPool;

        virtual void debug() {
        //    ImGui::Text(std::string("Type: " + typeid(*this).name()));
            ImGui::Text(std::string(name + " #" + std::to_string(id)).c_str());
        }
        GraphicsEntityInstance(RendererContent& rendererContent, GraphicsEntity* graphicsEntity) {
            this->graphicsEntity = graphicsEntity; 
            this->uniformBuffer = createUniformBuffer<T>(rendererContent);
            this->descriptorPool = createDescriptorPool<T>(rendererContent, *this);
        }
        virtual void render(RendererContent& rendererContent, const T& uniformBufferObject, int currentCommandBufferIndex) {

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
};
#endif