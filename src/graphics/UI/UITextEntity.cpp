#include "UITextEntity.h"
#include "../renderer.h"
#include "../shader.h"
#include "../font.h"
#include "../graphicsEntity.h"
#include <string>
#include "UIInstance.h"

const UITextEntity& createUITextEntity(Renderer& renderer, Shader* shader, Font* font) {

    const GraphicsEntity& graphicsEntity = createGraphicsEntity(renderer, getRenderPassObject(renderer, "UI"), shader, nullptr, &font->texture, false, sizeof(UIInstanceUniformBufferObject));
    UITextEntity* entity = new UITextEntity; 
    entity->font = font; 
    entity->vertexBuffer = graphicsEntity.vertexBuffer; 
    entity->texture = graphicsEntity.texture; 
    entity->shader = graphicsEntity.shader; 
    entity->depthTestEnabled = graphicsEntity.depthTestEnabled;
    entity->descriptorSetLayout = graphicsEntity.descriptorSetLayout; 
    entity->graphicsPipeline = graphicsEntity.graphicsPipeline;
    entity->pipelineLayout = graphicsEntity.pipelineLayout;

    std::remove(allGraphicsEntities.begin(), allGraphicsEntities.end(), &graphicsEntity);
    delete &graphicsEntity; 
    return *entity;
};
