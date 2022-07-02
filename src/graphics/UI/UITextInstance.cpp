#include "UITextInstance.h"
#include "../../core/predefined.h"
#include "../../utility/properties.h"
#include <string>
#include <math.h>

UITextInstance::UITextInstance(RendererContent& rendererContent, UITextEntity* entity) : UIInstance(rendererContent, entity) {
};

void UITextInstance::setCharacterSize(float newSize) {
    size.x = 0; 
    size.y = newSize;
};
void UITextInstance::render(RendererContent& rendererContent, int currentCommandBufferIndex) {

    VkCommandBuffer& commandBuffer = rendererContent.commandBuffers[currentCommandBufferIndex];
    VkDeviceSize offsets[] = {0};
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->graphicsEntity->graphicsPipeline);
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->graphicsEntity->pipelineLayout, 0, 1, &this->descriptorPool.descriptorSets[currentCommandBufferIndex], 0, nullptr);

    int width = PROPERTIES["windows"]["game"]["resolution"]["width"];
    int height = PROPERTIES["windows"]["game"]["resolution"]["height"];

    float currentAdvance = 0.0f;

    for(const char& character : text) {
        
        const Font& font = *(((UITextEntity*)graphicsEntity)->font); 
        const GlyphMetrics& metrics = font.allCharacterMetrics.at(character); 

        float sizeMultiplier = (size.y * ((float)height / (float)font.pixelSize)) * 1.4f;
        float descent = static_cast<float>(metrics.height - metrics.top);
        float x = currentAdvance + (metrics.width / 2.0f) + metrics.left; 
        float y = -descent + (metrics.height / 2.0f); 

        x = x / (float)width * sizeMultiplier; 
        y = y / (float)height * sizeMultiplier;
        x = (x - 0.5f) / 0.5f;
        y = (y - 0.5f) / 0.5f;
        x = x + 1; 
        y = y + 1; 
        x = x + position.x;
        y = y + position.y;

        glm::vec2 finalSize = {sizeMultiplier * metrics.width / (float)width, sizeMultiplier * metrics.height / (float)height};
        glm::vec2 finalPosition; 
        finalPosition.x = (float)x;
        finalPosition.y = (float)y;
        uniformBufferObject = {finalPosition, finalSize, color, hover() ? 1.0f : 0.0f};

        VkBuffer vertexBuffers[] = {font.allCharacterVertexBuffers.at(character).vertexBuffer};
        vkCmdPushConstants(commandBuffer, this->graphicsEntity->pipelineLayout, VK_SHADER_STAGE_VERTEX_BIT, 0, sizeof(UIInstanceUniformBufferObject), &uniformBufferObject); 
        vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
        vkCmdBindIndexBuffer(commandBuffer, font.allCharacterVertexBuffers.at(character).indexBuffer, 0, VK_INDEX_TYPE_UINT32);
        vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(font.allCharacterVertexBuffers.at(character).indices.size()), 1, 0, 0, 0);
        
        currentAdvance += (float)metrics.advance;
    }
}
void UITextInstance::debug() {
        ImGui::Text("UITextInstance"); 

        float arr[2] = {position.x, position.y};
        ImGui::InputFloat2("Position", arr);
        position = {arr[0], arr[1]};

        float sizeValue = size.y; 
        ImGui::InputFloat("Size", &sizeValue);
        size.y = sizeValue; 

        float colorArr[4] = {color.x, color.y, color.z, color.w};
        ImGui::ColorEdit4("Color", colorArr); 
        color = {colorArr[0], colorArr[1], colorArr[2], colorArr[3]};

        bool checks = hover();
        ImGui::Checkbox("Mouse hovering", &checks);

        int l = layer;
        ImGui::InputInt("Layer", &l);
        layer = l;

}