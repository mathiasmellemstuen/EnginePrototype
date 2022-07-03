#ifndef ENGINEPROTOTYPE_UIINSTANCE
#define ENGINEPROTOTYPE_UIINSTANCE

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "../graphicsEntityInstance.h"
#include "../uniformBufferObject.h"

struct UIInstanceUniformBufferObject {
    glm::vec2 position; 
    glm::vec2 size; 
    glm::vec4 color;
    float hover; 
};
class UIInstance : public GraphicsEntityInstance<UIInstanceUniformBufferObject> {
    public:
        glm::vec2 position; 
        glm::vec2 size; 
        glm::vec4 color;
        int layer; 
        virtual bool hover();
        virtual void debug();
        virtual void render(Renderer& renderer, int currentCommandBufferIndex);
        UIInstance(Renderer& renderer, GraphicsEntity* graphicsEntity);
};
#endif