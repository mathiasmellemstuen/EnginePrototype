#ifndef ENGINEPROTOTYPE_GENERIC_GRAPHICS_ENTITY_INSTANCE
#define ENGINEPROTOTYPE_GENERIC_GRAPHICS_ENTITY_INSTANCE
#include "../core/component.h"
#include "renderer.h"

class GenericGraphicsEntityInstance : public Component {
    public:
        RenderPassObject* renderPassObject;
        int layer; // For renderpasses that uses layers
        GenericGraphicsEntityInstance(RenderPassObject* renderPassObject);
        virtual void debug();
        virtual void reCreateGraphics(Renderer& renderer); 
        virtual void render(Renderer& renderer, int currentCommandBufferIndex);
};
#endif
