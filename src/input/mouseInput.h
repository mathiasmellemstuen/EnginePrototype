#ifndef ENGINEPROTOTYPE_MOUSE_INPUT
#define ENGINEPROTOTYPE_MOUSE_INPUT

#include <glm/vec2.hpp>
#include <SDL2/SDL.h>

class MouseInput {
    public:
        inline static glm::vec2 mousePosition; 
        static void update(SDL_Event& event);
};

#endif