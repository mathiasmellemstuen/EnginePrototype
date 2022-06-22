#include "UIInstance.h"
#include <glm/vec2.hpp>
#include "../../input/mouse.h"
#include <imgui/imgui.h>

UIInstance::UIInstance(RendererContent& rendererContent, GraphicsEntity* graphicsEntity) : GraphicsEntityInstance<UIInstanceUniformBufferObject>(rendererContent, graphicsEntity){

}

// This class can be any shape and therfore there is no way to know if you are hovering the item. Returning false. Can be overloaded in the future. 
bool UIInstance::hover() {
    return 0;
}

void UIInstance::debug() {
        //ImGui::Text(std::string(name + " #" + std::to_string(id)).c_str());
        ImGui::Text("UIInstance"); 

        float arr[2] = {position.x, position.y};
        ImGui::InputFloat2("Position", arr);
        position = {arr[0], arr[1]};

        float sizeArr[2] = {size.x, size.y};
        ImGui::InputFloat2("Size", sizeArr);
        size = {sizeArr[0], sizeArr[1]};

        float colorArr[4] = {color.x, color.y, color.z, color.w};
        ImGui::ColorEdit4("Color", colorArr); 
        color = {colorArr[0], colorArr[1], colorArr[2], colorArr[3]};

        bool checks = hover();
        ImGui::Checkbox("Mouse hovering", &checks);

        int l = layer;
        ImGui::InputInt("Layer", &l);
        layer = l;

}
void UIInstance::render(RendererContent& rendererContent, int currentCommandBufferIndex) {
    VkCommandBuffer& commandBuffer = rendererContent.commandBuffers[currentCommandBufferIndex];

    uniformBufferObject = {position, size, color, hover() ? 1.0f : 0.0f};
    updateUniformBuffer(rendererContent, this->uniformBuffer, uniformBufferObject, currentCommandBufferIndex);
    
    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->graphicsEntity->graphicsPipeline);
    
    VkBuffer vertexBuffers[] = {graphicsEntity->vertexBuffer->vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    
    vkCmdBindIndexBuffer(commandBuffer, graphicsEntity->vertexBuffer->indexBuffer, 0, VK_INDEX_TYPE_UINT32);
    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, this->graphicsEntity->pipelineLayout, 0, 1, &this->descriptorPool.descriptorSets[currentCommandBufferIndex], 0, nullptr);
    
    vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(graphicsEntity->vertexBuffer->indices.size()), 1, 0, 0, 0);

};