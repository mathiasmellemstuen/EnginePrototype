#include "UIRectangleInstance.h"
#include "../../core/predefined.h"
#include "../../input/mouse.h"

UIRectangleInstance::UIRectangleInstance(RendererContent& rendererContent) : UIInstance(rendererContent, &predefinedGraphicsEntities.uiRectangle) {

};

bool UIRectangleInstance::hover() {
    bool hoveringX = Mouse::mousePosition.x > position.x && Mouse::mousePosition.y < position.x + size.x;
    bool hoveringY = Mouse::mousePosition.y > position.y && Mouse::mousePosition.y < position.y + size.y; 

    return hoveringX && hoveringY;
}