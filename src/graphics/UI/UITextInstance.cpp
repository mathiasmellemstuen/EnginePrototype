#include "UITextInstance.h"
#include "../../core/predefined.h"
#include "../../utility/properties.h"
#include <string>

UITextInstance::UITextInstance(RendererContent& rendererContent, UITextEntity* entity) : UIInstance(rendererContent, entity) {
};

void UITextInstance::render(RendererContent& rendererContent, int currentCommandBufferIndex) {

    VkCommandBuffer& commandBuffer = rendererContent.commandBuffers[currentCommandBufferIndex];
    VkDeviceSize offsets[] = {0};
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->graphicsEntity->graphicsPipeline);
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->graphicsEntity->pipelineLayout, 0, 1, &this->descriptorPool.descriptorSets[currentCommandBufferIndex], 0, nullptr);

    int width = (*properties)["windows"]["game"]["resolution"]["width"];
    int height = (*properties)["windows"]["game"]["resolution"]["height"];
    float aspectRatio = (float)height / (float)width; 

    float x = position.x;

    for(const char& character : text) {

        const Font& font = *(((UITextEntity*)graphicsEntity)->font); 
        const GlyphMetrics& metrics = font.allCharacterMetrics.at(character); 

        float characterWidth = (float)metrics.width / width; 
        float characterHeight = (float)metrics.height / height; 

        glm::vec2 finalSize = {characterWidth, characterHeight}; 
        finalSize.x = finalSize.x * aspectRatio; 

        glm::vec2 finalPosition; 
        finalPosition.x = x + (metrics.bearingX / (float)width); // TODO: Needs fixing 
        finalPosition.y = position.y + (characterHeight); //TODO: Needs fixing 

        VkBuffer vertexBuffers[] = {font.allCharacterVertexBuffers.at(character).vertexBuffer};
        uniformBufferObject = {finalPosition, finalSize, color, hover() ? 1.0f : 0.0f};
        vkCmdPushConstants(commandBuffer, this->graphicsEntity->pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(UIInstanceUniformBufferObject), &uniformBufferObject); 
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(commandBuffer, font.allCharacterVertexBuffers.at(character).indexBuffer, 0, VK_INDEX_TYPE_UINT32);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(font.allCharacterVertexBuffers.at(character).indices.size()), 1, 0, 0, 0);
        
        x += (float)(metrics.advanceX >> 6) / (float)width;

    }
    
}; 