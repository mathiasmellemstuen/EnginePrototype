//
// Created by Mathias on 19/02/2022.
//

#ifndef ENGINEPROTOTYPE_GUIRENDERER_H
#define ENGINEPROTOTYPE_GUIRENDERER_H

#include "GUIElement.h"
#include <vector>
#include <glm/glm.hpp>
#include "../window.h"
#include "../uniformBuffer.h"

class GUIRenderer {
public:
    glm::mat4 projection;
    std::vector<GUIElement> elements;
    void update(const UniformBuffer& uniformBuffer, VkCommandBuffer& commandBuffer, int currentCommandBuffer, uint32_t currentImage);
    void resizeProjection(int screenWidth, int screenHeight);
    GUIRenderer();
};
#endif //ENGINEPROTOTYPE_GUIRENDERER_H