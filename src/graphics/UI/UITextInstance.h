#ifndef ENGINEPROTOTYPE_UI_TEXT_INSTANCE
#define ENGINEPROTOTYPE_UI_TEXT_INSTANCE

#include "UIRectangleInstance.h"
#include "../font.h"
#include "UITextEntity.h"

class UITextInstance : public UIInstance {
    public:
        std::string text;
        void setCharacterSize(float newSize); 
        virtual void render(Renderer& renderer, int currentCommandBufferIndex);
        virtual void debug(); 
        UITextInstance(Renderer& renderer, UITextEntity* entity);
};
#endif