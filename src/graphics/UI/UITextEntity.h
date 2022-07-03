#ifndef ENGINEPROTOTYPE_UI_TEXT_ENTITY
#define ENGINEPROTOTYPE_UI_TEXT_ENTITY

#include "../graphicsEntity.h"
#include "../font.h"

struct UITextEntity : GraphicsEntity {
    Font* font; 
};

const UITextEntity& createUITextEntity(Renderer& renderer, Shader* shader, Font* font);
#endif