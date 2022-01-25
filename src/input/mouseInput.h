#ifndef ENGINEPROTOTYPE_MOUSE_INPUT
#define ENGINEPROTOTYPE_MOUSE_INPUT

#include <glm/vec2.hpp>
#include <SDL2/SDL.h>

class MouseInput {
    public:
        inline static glm::vec2 mousePosition;
        inline static bool mouseLeftIsPressed;
        inline static bool mouseLeftOnDown; 
        inline static bool mouseLeftOnUp;
        inline static bool mouseRightIsPressed; 
        inline static bool mouseRightOnDown; 
        inline static bool mouseRightOnUp; 
        static void update(SDL_Event& event);
};

#endif