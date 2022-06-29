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

        virtual void render(RendererContent& rendererContent, int currentCommandBufferIndex);
        virtual void reCreateGraphics(RendererContent& rendererContent); 

        GraphicsEntityInstance(RendererContent& rendererContent, GraphicsEntity* graphicsEntity) {
            this->graphicsEntity = graphicsEntity; 
            this->uniformBuffer = createUniformBuffer<T>(rendererContent);
            this->descriptorPool = createDescriptorPool<T>(rendererContent, *this);
            this->uniformBufferObject = {};
        }
};

#include "graphicsEntityInstance.tpp"
#endif