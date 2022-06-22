#include <SDL2/SDL.h>
#include "mouse.h"
#include "../utility/debug.h"

void Mouse::update(SDL_Event& event) {

    // Updating mouse position
    if(event.type == SDL_MOUSEMOTION) {
        int x = event.motion.x;
        int y = event.motion.y;

        Mouse::mousePosition.x = x;
        Mouse::mousePosition.y = y;

        // Calculating the normalized mouse position
        int width; 
        int height; 
        SDL_GetWindowSize(SDL_GetWindowFromID(event.window.windowID), &width, &height); 
        Mouse::normalizedMousePosition.x = Mouse::mousePosition.x / (float)width; 
        Mouse::normalizedMousePosition.y = Mouse::mousePosition.y / (float)height;

        Mouse::mouseVulkanScreenPosition.x = Mouse::normalizedMousePosition.x < 0.5 ? ((Mouse::normalizedMousePosition.x - 0.5f) / 0.5f) : ((Mouse::normalizedMousePosition.x - 0.5f) / 0.5f);
        Mouse::mouseVulkanScreenPosition.y = Mouse::normalizedMousePosition.y < 0.5 ? ((Mouse::normalizedMousePosition.y - 0.5f) / 0.5f) : ((Mouse::normalizedMousePosition.y - 0.5f) / 0.5f);
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

void Mouse::frameUpdate() {

    int xRel = 0;
    int yRel = 0;
    SDL_GetRelativeMouseState(&xRel, &yRel);
    Mouse::mouseAcceleration.x = xRel;
    Mouse::mouseAcceleration.y = yRel;
}

void Mouse::enableRelativeMouse() {
    SDL_SetRelativeMouseMode(SDL_TRUE);
}

void Mouse::disableRelativeMouse() {
    SDL_SetRelativeMouseMode(SDL_FALSE);
}

bool Mouse::isMouseRelative() {
   return SDL_GetRelativeMouseMode();
}
