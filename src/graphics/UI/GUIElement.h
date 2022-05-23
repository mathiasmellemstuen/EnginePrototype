#ifndef ENGINEPROTOTYPE_GUIELEMENT_H
#define ENGINEPROTOTYPE_GUIELEMENT_H

#include <glm/glm.hpp>
#include <functional>
#include "../uniformBuffer.h"

class GUIElement {
public:
    glm::vec2 size;
    glm::vec2 position;
    virtual void update(const UniformBuffer& uniformBuffer, VkCommandBuffer& commandBuffer, int currentCommandBuffer, uint32_t currentImage) const;
    GUIElement(const glm::vec2& position, const glm::vec2& size);
};
#endif //ENGINEPROTOTYPE_GUIELEMENT_H
