#include "window.h"

#include <SDL2/SDL.h>
#include "../utility/debug.h"
#include <string>

Window::Window() {

    Debug::log(INFO, "Intializing SDL2"); 

    int code = SDL_Init(SDL_INIT_EVERYTHING);
    if(code < 0) {

        Debug::log(ERROR, "Error while initializing SDL2!"); 
        Debug::log(ERROR, "Error code " + std::to_string(code)); 
        std::string message = SDL_GetError();
        Debug::log(ERROR, "Error message: " + message);
    }
    Debug::log(SUCCESS, "SDL2 initiated!");
    Debug::log(INFO, "Creating SDL2 window context"); 
    //TODO: Change values in createWindow to properties values instead of hardcoded values. 
    sdlWindow = SDL_CreateWindow("Vulkan",SDL_WINDOWPOS_CENTERED,SDL_WINDOWPOS_CENTERED,1920,1080,SDL_WINDOW_VULKAN | SDL_WINDOW_SHOWN | SDL_WINDOW_RESIZABLE);
    Debug::log(SUCCESS, "Successfully created SDL window context!"); 
};

Window::~Window() {
    Debug::log(INFO, "Destroying SDL window");
    SDL_DestroyWindow(sdlWindow);
    sdlWindow = nullptr; 
    Debug::log(SUCCESS, "Successfully destroyed SDL window!"); 
};