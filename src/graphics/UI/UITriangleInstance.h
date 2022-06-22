#ifndef ENGINEPROTOTYPE_UI_TRIANGLE_INSTANCE
#define ENGINEPROTOTYPE_UI_TRIANGLE_INSTANCE

#include "UIInstance.h"
#include "../renderer.h"

class UITriangleInstance : public UIInstance {
    public:
        UITriangleInstance(RendererContent& rendererContent);
        virtual bool hover();
};
#endif