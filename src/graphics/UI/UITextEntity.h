#ifndef ENGINEPROTOTYPE_UI_TEXT_ENTITY
#define ENGINEPROTOTYPE_UI_TEXT_ENTITY

#include "../graphicsEntity.h"
#include "../font.h"

struct UITextEntity : GraphicsEntity {
    Font* font; 
};

const UITextEntity& createUITextEntity(RendererContent& rendererContent, Shader* shader, Font* font);
#endif