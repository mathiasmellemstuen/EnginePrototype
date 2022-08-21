#include "UITriangleInstance.h"
#include "../../core/predefined.h"
#include "../../input/mouse.h"
#include <math.h>
#include <string>

UITriangleInstance::UITriangleInstance(Renderer& renderer) : UIInstance(renderer, getRenderPassObject(renderer, "UI"), &predefinedGraphicsEntities.uiTriangle) {

};

bool UITriangleInstance::hover() {
    bool hoveringX = Mouse::mouseVulkanScreenPosition.x > position.x - size.x && Mouse::mouseVulkanScreenPosition.x < position.x + size.x;
    bool hoveringY = Mouse::mouseVulkanScreenPosition.y > position.y - size.y && Mouse::mouseVulkanScreenPosition.y < position.y + size.y; 
    
    float normalizedX = (position.x - Mouse::mouseVulkanScreenPosition.x) / -size.x; 
    float normalizedY = (position.y - Mouse::mouseVulkanScreenPosition.y) / -size.y; 
    float allowedY = size.y - abs(normalizedX) * size.y; 
    float finalY = ((normalizedY < 0 ? (-1 * normalizedY) + 1 : 1 - normalizedY) / 2.0) * size.y;
    
    bool triangleCondition = finalY < allowedY; 

    return !Mouse::isMouseRelative() && hoveringX && hoveringY && triangleCondition; 
}
