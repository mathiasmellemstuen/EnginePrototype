#include "GUIRenderer.h"
#include <glm/gtc/matrix_transform.hpp>

GUIRenderer::GUIRenderer() {
    projection = glm::ortho(0, 1920, 1080, 0);
};

void GUIRenderer::resizeProjection(int screenWidth, int screenHeight) {
    projection = glm::ortho(0, screenWidth, screenHeight, 0);
};
void GUIRenderer::update(const UniformBuffer& uniformBuffer, VkCommandBuffer& commandBuffer, int currentCommandBuffer, uint32_t currentImage) {

    for(int i = 0; i < elements.size(); i++) {
        elements[i].update();
    }
}