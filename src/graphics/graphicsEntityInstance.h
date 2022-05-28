//
// Created by Mathias on 15.02.2022.
//

#ifndef ENGINEPROTOTYPE_GRAPHICSENTITYINSTANCE
#define ENGINEPROTOTYPE_GRAPHICSENTITYINSTANCE

#include "uniformBuffer.h"
#include "graphicsEntity.h"
#include "renderer.h"
#include "../core/component.h"
#include "descriptorPool.h"

class GraphicsEntityInstance : public Component {
    public:
        GraphicsEntity* graphicsEntity;
        UniformBuffer uniformBuffer;
        DescriptorPool descriptorPool;
        virtual void render(RendererContent& rendererContent, glm::mat4& view, glm::mat4& projection, glm::mat4& model, int currentCommandBufferIndex);
        virtual void debug();
        GraphicsEntityInstance(RendererContent& rendererContent, GraphicsEntity* graphicsEntity);
};
#endif