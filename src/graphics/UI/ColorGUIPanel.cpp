#include "ColorGUIPanel.h"
#include "GUIRenderer.h"

ColorGUIPanel::ColorGUIPanel(const glm::vec2 &position, const glm::vec2 &size, const glm::vec4 &color) : GUIElement(position, size) {
    this->color = color;
}

void ColorGUIPanel::update(GUIRenderer& guiRenderer,VkCommandBuffer& commandBuffer, int currentCommandBuffer, uint32_t currentImage) const {
    glm::mat4 identity(1);


    // Find a replacement for this function
    // uniformBuffer.update(currentImage, identity, guiRenderer.projection, identity);

    vkCmdBindPipeline(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, graphicsPipeline);

    VkBuffer vertexBuffers[] = {vertexBuffer};
    VkDeviceSize offsets[] = {0};
    vkCmdBindVertexBuffers(commandBuffer, 0, 1, vertexBuffers, offsets);
    vkCmdBindIndexBuffer(commandBuffer, indexBuffer, 0, VK_INDEX_TYPE_UINT32);

    vkCmdBindDescriptorSets(commandBuffer, VK_PIPELINE_BIND_POINT_GRAPHICS, pipelineLayout, 0, 1, &descriptorSets[currentCommandBuffer], 0, nullptr);

    // Find a replacement for this without using indices
    //vkCmdDrawIndexed(commandBuffer, static_cast<uint32_t>(indices.size()), 1, 0, 0, 0);
};