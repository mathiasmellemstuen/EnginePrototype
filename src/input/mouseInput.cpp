#include <SDL2/SDL.h>
#include "mouseInput.h"
#include "../utility/debug.h"

void MouseInput::update(SDL_Event& event) {
    
    // Updating mouse position
    if(event.type == SDL_MOUSEMOTION) {
        int x = event.motion.x;
        int y = event.motion.y;

        MouseInput::mousePosition.x = x; 
        MouseInput::mousePosition.y = y; 
    }
    
    // Registering left clicks
    mouseLeftOnDown = event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_LEFT; 
    mouseLeftOnUp = event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_LEFT; 
    mouseLeftIsPressed = mouseLeftOnDown ? true : mouseLeftOnUp ? false : mouseLeftIsPressed;
    
    // Registering right clicks
    mouseRightOnDown = event.type == SDL_MOUSEBUTTONDOWN && event.button.button == SDL_BUTTON_RIGHT; 
    mouseRightOnUp = event.type == SDL_MOUSEBUTTONUP && event.button.button == SDL_BUTTON_RIGHT; 
    mouseRightIsPressed = mouseRightOnDown ? true : mouseRightOnUp ? false : mouseRightIsPressed;
}