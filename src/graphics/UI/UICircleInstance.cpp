#include "UICircleInstance.h"
#include "../../core/predefined.h"
#include "../../utility/properties.h"
#include "../../input/mouse.h"

UICircleInstance::UICircleInstance(Renderer& renderer) : UIInstance(renderer, getRenderPassObject(renderer, "UI"), &predefinedGraphicsEntities.uiCircle) {

};

bool UICircleInstance::hover() {
    bool hoveringX = Mouse::mouseVulkanScreenPosition.x > position.x - size.x && Mouse::mouseVulkanScreenPosition.x < position.x + size.x;
    bool hoveringY = Mouse::mouseVulkanScreenPosition.y > position.y - size.y && Mouse::mouseVulkanScreenPosition.y < position.y + size.y; 
    float e1 = (abs(Mouse::mouseVulkanScreenPosition.x) - position.x) / size.x; 
    float e2 = (abs(Mouse::mouseVulkanScreenPosition.y) - position.y) / size.y; 
    float d = (e1 * e1) + (e2 * e2);

    return d < 1.0f; 
}

void UICircleInstance::setRadius(float radius) {

    int width = (*properties)["windows"]["game"]["resolution"]["width"];
    int height = (*properties)["windows"]["game"]["resolution"]["height"];
    float aspectRatio = (float)height / (float)width; 

    size.x = radius * aspectRatio; 
    size.y = radius;
}
