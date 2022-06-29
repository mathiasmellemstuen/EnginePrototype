#ifndef ENGINEPROTOTYPE_GENERIC_GRAPHICS_ENTITY_INSTANCE
#define ENGINEPROTOTYPE_GENERIC_GRAPHICS_ENTITY_INSTANCE
#include "../core/component.h"
#include "renderer.h"

class GenericGraphicsEntityInstance : public Component {
    public: 
        virtual void debug();
        virtual void reCreateGraphics(RendererContent& rendererContent); 
        virtual void render(RendererContent& rendererContent, int currentCommandBufferIndex);
};
#endif