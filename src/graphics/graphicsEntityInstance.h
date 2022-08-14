#ifndef ENGINEPROTOTYPE_GRAPHICSENTITYINSTANCE
#define ENGINEPROTOTYPE_GRAPHICSENTITYINSTANCE

#include "uniformBuffer.h"
#include "graphicsEntity.h"
#include "renderer.h"
#include "../core/component.h"
#include "uniformBufferObject.h"
#include "descriptorPool.h"
#include "genericGraphicsEntityInstance.h"

template<typename T> class GraphicsEntityInstance : public GenericGraphicsEntityInstance {
    public:
        GraphicsEntity* graphicsEntity;
        UniformBuffer uniformBuffer;
        DescriptorPool descriptorPool;
        T uniformBufferObject;

        virtual void render(Renderer& renderer, int currentCommandBufferIndex);
        virtual void reCreateGraphics(Renderer& renderer);

        GraphicsEntityInstance(Renderer& renderer, RenderPassObject* renderPassObject, GraphicsEntity* graphicsEntity) : GenericGraphicsEntityInstance(renderer, renderPassObject->name) {
            this->graphicsEntity = graphicsEntity;
            this->uniformBuffer = createUniformBuffer<T>(renderer);
            this->descriptorPool = createDescriptorPool<T>(renderer, *this);
            this->uniformBufferObject = {};
        }
};

#include "graphicsEntityInstance.tpp"
#endif
