#include "UITriangleInstance.h"
#include "../../core/predefined.h"
#include "../../input/mouse.h"

UITriangleInstance::UITriangleInstance(RendererContent& rendererContent) : UIInstance(rendererContent, &predefinedGraphicsEntities.uiTriangle) {

};

bool UITriangleInstance::hover() {
    return 0; 
}