#ifndef ENGINEPROTOTYPE_UIINSTANCE
#define ENGINEPROTOTYPE_UIINSTANCE

#include <glm/vec2.hpp>
#include <glm/vec3.hpp>
#include "../graphicsEntity.h"

class UIInstance {
    public:
        glm::vec2 position; 
        glm::vec2 size; 
        glm::vec3 color;
        bool hover();
        virtual void render();
        virtual void update();
        UIInstance(GraphicsEntity* graphicsEntity);
};
#endif