#include "UIRectangleInstance.h"
#include "../../core/predefined.h"
#include "../../input/mouse.h"

UIRectangleInstance::UIRectangleInstance(RendererContent& rendererContent) : UIInstance(rendererContent, &predefinedGraphicsEntities.uiRectangle) {
};

bool UIRectangleInstance::hover() {
    bool hoveringX = Mouse::mouseVulkanScreenPosition.x > position.x - size.x && Mouse::mouseVulkanScreenPosition.x < position.x + size.x;
    bool hoveringY = Mouse::mouseVulkanScreenPosition.y > position.y - size.y && Mouse::mouseVulkanScreenPosition.y < position.y + size.y; 

    return !Mouse::isMouseRelative() && hoveringX && hoveringY;
}