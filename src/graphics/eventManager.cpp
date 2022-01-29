#include "eventManager.h"
#include "../utility/debug.h"
#include "../input/mouseInput.h"
#include "../input/keyboardInput.h"
#include "window.h"
#include <SDL2/SDL.h>
#include <imgui/imgui.h>

void EventManager::update(SDL_Window& window) {

        KeyboardInput::frameUpdate(); 

        while(SDL_PollEvent(&event)) {

            MouseInput::update(event);
            KeyboardInput::update(event); 
            
            #ifndef NOTDEBUG
                if(event.window.windowID != SDL_GetWindowID(&window))
                    ImGui_ImplSDL2_ProcessEvent(&event);
            #endif 
            
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