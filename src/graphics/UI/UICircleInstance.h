#ifndef ENGINEPROTOTYPE_UI_CIRCLE
#define ENGINEPROTOTYPE_UI_CIRCLE

#include "UIInstance.h"
#include "../renderer.h"

class UICircleInstance : public UIInstance {
    public:
        UICircleInstance(Renderer& renderer);
        virtual bool hover();
        virtual void setRadius(float radius); 
};

#endif
