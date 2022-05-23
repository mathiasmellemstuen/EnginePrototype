//
// Created by Mathias on 19/02/2022.
//

#ifndef ENGINEPROTOTYPE_COLORGUIPANEL_H
#define ENGINEPROTOTYPE_COLORGUIPANEL_H

#include "GUIElement.h"
#include <vulkan/vulkan.h>

class GUIRenderer;

class ColorGUIPanel : public GUIElement {
public:
    // Pipeline
    static VkPipeline graphicsPipeline;
    static VkPipelineLayout pipelineLayout;

    // Uniforms
    std::vector<VkBuffer> uniformBuffers;
    std::vector<VkDeviceMemory> uniformBuffersMemory;

    // Vertices and indices.
    VkBuffer vertexBuffer;
    VkBuffer indexBuffer;
    VkDeviceMemory vertexBufferMemory;
    VkDeviceMemory indexBufferMemory;

    //Descriptors
    VkDescriptorPool descriptorPool;
    std::vector<VkDescriptorSet> descriptorSets;

    glm::mat4 model;

    glm::vec4 color;

    void update(GUIRenderer& guiRenderer, VkCommandBuffer& commandBuffer, int currentCommandBuffer, uint32_t currentImage) const override;
    ColorGUIPanel(const glm::vec2& position, const glm::vec2& size, const glm::vec4& color);
};

#endif //ENGINEPROTOTYPE_COLORGUIPANEL_H
