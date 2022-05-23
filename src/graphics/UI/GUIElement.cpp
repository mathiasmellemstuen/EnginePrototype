//
// Created by Mathias on 19/02/2022.
//
#include "GUIElement.h"

GUIElement::GUIElement(const glm::vec2& position, const glm::vec2& size) {
    this->position = position;
    this->size = size;
};

void GUIElement::update(const UniformBuffer& uniformBuffer, VkCommandBuffer& commandBuffer, int currentCommandBuffer, uint32_t currentImage) const {

}