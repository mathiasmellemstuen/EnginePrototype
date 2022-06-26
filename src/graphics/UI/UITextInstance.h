#ifndef ENGINEPROTOTYPE_UI_TEXT_INSTANCE
#define ENGINEPROTOTYPE_UI_TEXT_INSTANCE

#include "UIRectangleInstance.h"

class UITextInstance : public UIInstance {
    public:
        std::string text;
        UITextInstance(RendererContent& RendererContent);
};
#endif