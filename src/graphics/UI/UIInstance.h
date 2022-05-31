#ifndef ENGINEPROTOTYPE_UIINSTANCE
#define ENGINEPROTOTYPE_UIINSTANCE

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "../graphicsEntityInstance.h"

struct UIInstanceUniformBufferObject {
    glm::vec2 position; 
    glm::vec2 size; 
    glm::vec3 color;
    bool hover; 
};
class UIInstance : public GraphicsEntityInstance<UIInstanceUniformBufferObject> {
    public:
        glm::vec2 position; 
        glm::vec2 size; 
        glm::vec3 color;
        bool hover();
        UIInstance(RendererContent& rendererContent, GraphicsEntity* graphicsEntity);
};
#endif