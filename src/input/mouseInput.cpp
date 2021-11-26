#include <SDL2/SDL.h>
#include "mouseInput.h"

void MouseInput::update(SDL_Event& event) {
    if(event.type == SDL_MOUSEMOTION) {
        int x = event.motion.x;
        int y = event.motion.y;

        MouseInput::mousePosition.x = x; 
        MouseInput::mousePosition.y = y; 
    }
}