#ifndef ENGINEPROTOTYPE_UI_RECTANGLE_INSTANCE
#define ENGINEPROTOTYPE_UI_RECTANGLE_INSTANCE

#include "UIInstance.h"
#include "../renderer.h"

class UIRectangleInstance : public UIInstance {
    public:
        UIRectangleInstance(Renderer& renderer);
        virtual bool hover();
};

#endif