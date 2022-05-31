#include "UIInstance.h"
#include <glm/vec2.hpp>
#include "../../input/mouse.h"

UIInstance::UIInstance(RendererContent& rendererContent, GraphicsEntity* graphicsEntity) : GraphicsEntityInstance<UIInstanceUniformBufferObject>(rendererContent, graphicsEntity){

}

bool UIInstance::hover() {
    return !Mouse::isMouseRelative() && Mouse::mousePosition.x > position.x && Mouse::mousePosition.x < (position.x + size.x) && Mouse::mousePosition.y > position.y && Mouse::mousePosition.y > (position.y + size.y);
}