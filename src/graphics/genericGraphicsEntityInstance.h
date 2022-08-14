#ifndef ENGINEPROTOTYPE_GENERIC_GRAPHICS_ENTITY_INSTANCE
#define ENGINEPROTOTYPE_GENERIC_GRAPHICS_ENTITY_INSTANCE
#include "../core/component.h"

#include "renderer.h"

class GenericGraphicsEntityInstance : public Component {
    public:
        RenderPassObject* renderPassObject;
		uint16_t layer; // For renderpasses that uses layer
		void setLayer(uint16_t newLayer); 
        GenericGraphicsEntityInstance(Renderer& renderer, const std::string& renderPassName);
        virtual void debug();
        virtual void reCreateGraphics(Renderer& renderer); 
        virtual void render(Renderer& renderer, int currentCommandBufferIndex);
};
#endif
