#include "eventManager.h"
#include "../utility/debug.h"
#include "../utility/logging.h"
#include "../input/mouse.h"
#include "../input/keyboardInput.h"
#include "window.h"
#include <SDL2/SDL.h>
#include <imgui/imgui.h>
#include <iostream>
#include "window.h"

void EventManager::update(Window& window) {

        KeyboardInput::frameUpdate();
        Mouse::frameUpdate();

    while(SDL_PollEvent(&event)) {

            #ifndef NO_DEBUG_WINDOW
            //This line is causing problems where the game screen is black?
            if(Debug::updateDebugWindowEvents(event)) {
                return;
            }
            #endif

            Mouse::update(event);
            KeyboardInput::update(event);

            if (event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_CLOSE) {

                window.running = false;
                
                logger(INFO, "SDL_Quit event is happening.");
                if(Debug::debugWindowRunning) {
                    Debug::cleanupDebugWindow();
                    Debug::debugWindowRunning = false;
                }
                return; 
            }
            if (window.running && event.type == SDL_WINDOWEVENT && event.window.event == SDL_WINDOWEVENT_RESIZED && event.window.windowID == SDL_GetWindowID(window.sdlWindow)) {
                
                //Window is resized, re creating swapchain: 
                window.framebufferResized = true;
            }
        }
}