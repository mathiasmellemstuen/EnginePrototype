#include "eventManager.h"
#include "../utility/debug.h"
#include "../input/mouseInput.h"
#include "window.h"
#include <SDL2/SDL.h>

void EventManager::update(SDL_Window& window) {

        while(SDL_PollEvent(&event)) {

            MouseInput::update(event);

            if(event.type == SDL_QUIT) {
                // window.running = false;
                
                Debug::log("SDL_Quit event is happening."); 
                if(Debug::debugWindowRunning) {
                    Debug::cleanupDebugWindow();
                }
            }
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE && event.window.windowID == SDL_GetWindowID(&window)) {

                // window.running = false;
                
                Debug::log("SDL_Quit event is happening."); 
                if(Debug::debugWindowRunning) {
                    Debug::cleanupDebugWindow();
                }
            }
            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED && event.window.windowID == SDL_GetWindowID(&window)) {

                //Window is resized, re creating swapchain: 
                // window.framebufferResized = true; 
            }
        }
}